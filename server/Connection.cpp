#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>

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

Connection::Connection(Server* server, ConnectionManager* manager, struct epoll_event* event)
    : m_server(server)
    , m_manager(manager)
    , m_event(event)
{
    // m_clientSocket = server->connect();
    m_clientSocket = accept(server->getSocket(), NULL, NULL);
}

Connection::~Connection()
{
    ::close(m_clientSocket);
}

void Connection::notifyEvent()
{
    if (m_event->events & EPOLLIN)
    {
        // HTTPRequest req = new HTTPRequest(read());
        // m_server->receive(request, response);
        m_server->receive(read());
    }
    else if (m_event->events & EPOLLOUT)
    {
        m_server->send("entrada de teste");
        // m_server->send(request, response);
    }
    // m_server->handleRequest(request, response);
    m_server->handleIncomingData(this);
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
