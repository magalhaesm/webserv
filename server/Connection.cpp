#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

#include "Connection.hpp"
#include "Dispatcher.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

Connection::Connection(Server* server, Dispatcher* dispatcher)
    : m_server(server)
    , m_dispatcher(dispatcher)
    , m_request(NULL)
    , m_response(NULL)
{
    m_clientSocket = server->accept();
}

Connection::~Connection()
{
    delete m_request;
    delete m_response;
    ::close(m_clientSocket);
}

HTTPRequest* Connection::request()
{
    if (m_request == NULL)
    {
        m_request = new HTTPRequest(this);
    }
    return m_request;
}

HTTPResponse* Connection::response()
{
    if (m_response == NULL)
    {
        m_response = new HTTPResponse(this);
    }
    return m_response;
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
        this->write(m_response->toString());
        if (finished)
        {
            this->close();
        }
    }
}

void Connection::close()
{
    m_dispatcher->close(this);
}

// TODO: handle errors
std::string Connection::read()
{
    std::string request;
    request.resize(BUFSIZ);
    ::read(m_clientSocket, &request[0], request.size());
    return request;
}

// TODO: handle errors
void Connection::write(const std::string& response)
{
    ::write(m_clientSocket, &response[0], response.size());
}

int Connection::getSocket() const
{
    return m_clientSocket;
}
