#include <cstdio>
#include <iostream>
#include <strings.h>
#include <unistd.h>
#include <sys/socket.h>

#include "Connection.hpp"
#include "EventListener.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

const int BUFSIZE = 1024;

Connection::Connection(EventListener* listener, Server* server)
    : m_server(server)
    , m_listener(listener)
    , m_request(NULL)
    , m_response(NULL)
    , m_parser(server->getParser())
    , m_lastActivityTime(time(NULL))
{
    std::cout << "== NEW CONNECTION ==\n";
    m_clientSocket = server->accept();
}

Connection::~Connection()
{
    delete m_request;
    delete m_response;
    ::close(m_clientSocket);
}

// +--------------------+         +--------------------+
// | APPLICATION (send) |         | APPLICATION (recv) |
// +--------------------+         +--------------------+
// |      KERNEL        |   -->   |      KERNEL        |
// +--------------------+         +--------------------+

// XXX: POST
// content-length, content-type

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

    // if (m_parser.isRequestComplete(m_buffer))
    // {
    //     m_request = new HTTPRequest(m_buffer);
    //     m_server->handleRequest(m_request, response());
    //     m_buffer = m_response->toString();
    //     updateLastActivityTime();
    //     return true;
    // }

    if (m_parser.isRequestComplete(m_buffer))
    {
        HTTPRequest request = m_parser.newHTTPRequest(); // TODO: request: canonical form
        // m_response.clear();
        m_server->handleRequest(&request, response());
        m_buffer = m_response->toString();
        updateLastActivityTime();
        return true;
    }
    // size_t pos = m_buffer.rfind("\r\n\r\n");
    // if (pos != std::string::npos)
    // {
    //     m_request = new HTTPRequest(m_buffer);
    //     m_server->handleRequest(m_request, response());
    //     m_buffer = m_response->toString();
    //     updateLastActivityTime();
    //     return true;
    // }
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
        return m_response->isPersistent() ? true : this->close();
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

// TODO: manter o buffer de entrada na conexão
// const HTTPRequest* Connection::request()
// {
//     if (!m_request)
//     {
//         m_request = new HTTPRequest("");
//     }
//     return m_request;
// }

HTTPResponse* Connection::response()
{
    if (!m_response)
    {
        m_response = new HTTPResponse(this);
    }
    return m_response;
}

void Connection::updateLastActivityTime()
{
    m_lastActivityTime = time(NULL);
}
