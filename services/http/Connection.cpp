#include <unistd.h>
#include <strings.h>
#include <sys/socket.h>

#include "Connection.hpp"
#include "HTTPParser.hpp"
#include "EventListener.hpp"

const int BUFSIZE = 10;
// const int BUFSIZE = 4096;

// TODO: usar clear na Message como função da classe Connection
// TODO: receber do servidor as restrições de parsing
Connection::Connection(EventListener* listener, Server* server)
    : m_server(server)
    , m_listener(listener)
    , m_lastActivityTime(time(NULL))
    , m_persistent(true)
{
    m_clientSocket = server->accept();
}

Connection::~Connection()
{
    ::close(m_clientSocket);
}

void Connection::processRequest()
{
    HTTPRequest request(m_msg);
    HTTPResponse response;
    m_server->handleRequest(request, response);
    this->setPersistent(response.isKeepAlive());
    this->send(response.toString());
}

bool Connection::read()
{
    char buffer[BUFSIZE];
    ssize_t bytesRead = ::recv(m_clientSocket, buffer, BUFSIZE, 0);
    if (bytesRead <= 0)
    {
        return this->close();
    }

    m_raw.append(buffer, bytesRead);

    if (HTTPParser::parseRequest(m_raw, m_msg) == false)
    {
        return false;
    }

    this->processRequest();
    this->updateLastActivityTime();
    return true;
}

bool Connection::write()
{
    if (m_raw.empty())
    {
        return true;
    }

    size_t bufsiz = std::min<int>(m_raw.size(), BUFSIZE);
    ssize_t bytesWritten = ::send(m_clientSocket, m_raw.c_str(), bufsiz, 0);
    if (bytesWritten <= 0)
    {
        return this->close();
    }

    this->updateLastActivityTime();
    m_raw.erase(0, bytesWritten);

    if (m_raw.empty())
    {
        return m_persistent ? true : this->close();
    }
    return false;
}

inline void Connection::send(const std::string& response)
{
    m_raw = response;
    clear(m_msg);
}

bool Connection::close()
{
    m_listener->close(this);
    return true;
}

int Connection::getID() const
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
