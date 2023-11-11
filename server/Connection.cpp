#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

class Server;

#include "Connection.hpp"
#include "ConnectionManager.hpp"

Connection::Connection(Server* server, ConnectionManager* manager)
    : m_server(server)
    , m_manager(manager)
{
    // m_clientSocket = server->connect();
    m_clientSocket = accept(server->getSocket(), NULL, NULL);
}

Connection::~Connection()
{
    ::close(m_clientSocket);
}

std::string* Connection::getRequest()
{
    return &m_request;
}

std::string* Connection::getResponse()
{
    return &m_response;
}

void Connection::notify(struct epoll_event* event)
{
    if (event->events & EPOLLIN)
    {
        bool finished = m_server->read(this);
        if (finished)
        {
            event->events = EPOLLOUT | EPOLLET;
        }
    }
    if (event->events & EPOLLOUT)
    {
        bool finished = m_server->write(this);
        this->write(m_response);
        if (finished)
        {
            this->close();
        }
    }
}

void Connection::close()
{
    m_manager->close(this);
}

// TODO: throw error
std::string Connection::read()
{
    std::string request;
    request.resize(BUFSIZ);
    ::read(m_clientSocket, &request[0], request.size());
    return request;
}

// TODO: throw error
void Connection::write(const std::string& response)
{
    ::write(m_clientSocket, &response[0], response.size());
}

int Connection::getSocket() const
{
    return m_clientSocket;
}
