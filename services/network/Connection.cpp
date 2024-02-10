#include <cstdio>
#include <iostream>
#include <strings.h>
#include <unistd.h>
#include <sys/socket.h>

#include "Connection.hpp"
#include "EventListener.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

const int BUFSIZE = 10;

Connection::Connection(EventListener* listener, Server* server)
    : m_server(server)
    , m_listener(listener)
    , m_lastActivityTime(time(NULL))
    , m_persistent(true)
{
    std::cout << "== NEW CONNECTION ==\n";
    m_clientSocket = server->accept();
}

Connection::~Connection()
{
    ::close(m_clientSocket);
}

// TODO: lançar exceção dentro do controller
// erros podem ser de leitura, escrita e de parsing no http
bool Connection::read()
{
    char buffer[BUFSIZE];
    ssize_t bytesRead = ::recv(m_clientSocket, buffer, BUFSIZE, 0);
    if (bytesRead <= 0)
    {
        return this->close();
    }

    m_buffer.append(buffer, bytesRead);

    // TODO: refactor: conexão não conhece o protocolo que trafega
    // server.parseRequest(m_buffer);
    // server.processRequest(this);
    if (m_server->parser().isRequestComplete(m_buffer))
    {
        HTTPRequest request = m_server->parser().newHTTPRequest();
        HTTPResponse response;
        m_server->handleRequest(request, response);
        this->setPersistent(response.isKeepAlive());
        m_buffer = response.toString();
        this->updateLastActivityTime();
        return true;
    }
    return false;
}

bool Connection::write()
{
    if (m_buffer.empty())
    {
        return true;
    }

    size_t bufsiz = std::min<int>(m_buffer.size(), BUFSIZE);
    ssize_t bytesWritten = ::send(m_clientSocket, m_buffer.c_str(), bufsiz, 0);
    if (bytesWritten <= 0)
    {
        return this->close();
    }

    updateLastActivityTime();
    m_buffer.erase(0, bytesWritten);

    if (m_buffer.empty())
    {
        return m_persistent ? true : this->close();
    }
    return false;
}

bool Connection::close()
{
    m_listener->close(this);
    return true;
}

int Connection::getSocket() const
{
    return m_clientSocket;
}

std::time_t Connection::getLastActivityTime() const
{
    return m_lastActivityTime;
}

void Connection::setPersistent(bool persistent)
{
    m_persistent = persistent;
}

void Connection::updateLastActivityTime()
{
    m_lastActivityTime = time(NULL);
}
