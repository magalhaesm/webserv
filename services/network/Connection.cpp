#include <unistd.h>
#include <strings.h>
#include <sys/socket.h>

#include "Connection.hpp"
#include "EventListener.hpp"

const int BUFSIZE = 10;

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

bool Connection::read()
{
    char buffer[BUFSIZE];
    ssize_t bytesRead = ::recv(m_clientSocket, buffer, BUFSIZE, 0);
    if (bytesRead <= 0)
    {
        return this->close();
    }

    m_buffer.append(buffer, bytesRead);

    if (m_server->parseRequest(m_buffer))
    {
        m_server->processRequest(this);
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

    this->updateLastActivityTime();
    m_buffer.erase(0, bytesWritten);

    if (m_buffer.empty())
    {
        return m_persistent ? true : this->close();
    }
    return false;
}

void Connection::send(const std::string& response)
{
    m_buffer = response;
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
