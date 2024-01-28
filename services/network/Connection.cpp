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
// [ ] tempo-limite excedido
void Connection::notify(struct epoll_event* event)
{
    if (event->events & (EPOLLRDHUP | EPOLLERR))
    {
        this->close();
        return;
    }
    if (event->events & EPOLLIN)
    {
        m_server->handleRequest(request(), response());
        event->events = EPOLLOUT | EPOLLET;
    }
    if (event->events & EPOLLOUT)
    {
        this->write(m_response);
        this->close();
    }
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
    return request;
}

void Connection::write(HTTPResponse* response)
{
    const std::string& stream = response->toString();
    ::write(m_clientSocket, stream.c_str(), stream.size());
}

int Connection::getSocket() const
{
    return m_clientSocket;
}
