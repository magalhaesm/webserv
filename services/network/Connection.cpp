#include <cstdio>
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
    , m_timeOfLastActivity(time(NULL))
{
    m_clientSocket = server->accept();
}

Connection::~Connection()
{
    delete m_request;
    delete m_response;
    ::close(m_clientSocket);
}

const HTTPRequest* Connection::request()
{
    if (!m_request)
    {
        m_request = new HTTPRequest(this->read());
    }
    return m_request;
}

HTTPResponse* Connection::response()
{
    if (!m_response)
    {
        m_response = new HTTPResponse(this);
    }
    return m_response;
}

// TODO:
// [x] desconexão do cliente
// [x] tempo-limite excedido
void Connection::notify(struct epoll_event* event)
{
    if (event->events & (EPOLLRDHUP | EPOLLERR))
    {
        close();
        return;
    }
    if (event->events & EPOLLIN)
    {
        m_server->handleRequest(request(), response());
        event->events = EPOLLOUT;
    }
    if (event->events & EPOLLOUT)
    {
        write(m_response);
        close();
    }
}

std::time_t Connection::getLastActivity() const
{
    return m_timeOfLastActivity;
}

void Connection::close()
{
    m_dispatcher->close(this);
}

// TODO: lançar exceção dentro do controller
// erros podem ser de leitura, escrita e de parsing no http
std::string Connection::read()
{
    std::string request;
    request.resize(BUFSIZ);
    if (::recv(m_clientSocket, &request[0], request.size(), 0) <= 0)
    {
        close();
    }
    m_timeOfLastActivity = time(NULL);
    return request;
}

void Connection::write(HTTPResponse* response)
{
    const std::string& stream = response->toString();
    if (::send(m_clientSocket, stream.c_str(), stream.size(), 0) <= 0)
    {
        close();
    }
    m_timeOfLastActivity = time(NULL);
}

int Connection::getSocket() const
{
    return m_clientSocket;
}
