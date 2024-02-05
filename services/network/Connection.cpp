#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>

#include "Connection.hpp"
#include "EventListener.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

Connection::Connection(Server* server, EventListener* listener)
    : m_server(server)
    , m_request(NULL)
    , m_response(NULL)
    , m_listener(listener)
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

// TODO: lançar exceção dentro do controller
// erros podem ser de leitura, escrita e de parsing no http
bool Connection::read()
{
    std::string input;
    input.resize(BUFSIZ + 1);
    if (::recv(m_clientSocket, &input[0], input.size(), 0) <= 0)
    {
        close();
    }

    if (!m_request)
    {
        m_request = new HTTPRequest(input);
    }
    m_server->handleRequest(m_request, response());
    updateLastActivity();

    return true;
}

bool Connection::write()
{
    HTTPResponse* resp = m_response;
    const std::string& stream = resp->toString();
    if (::send(m_clientSocket, stream.c_str(), stream.size(), 0) <= 0)
    {
        close();
    }
    updateLastActivity();

    close();
    return false;
}

void Connection::close()
{
    m_listener->close(this);
}

int Connection::getSocket() const
{
    return m_clientSocket;
}

std::time_t Connection::getLastActivity() const
{
    return m_timeOfLastActivity;
}

// TODO: manter o buffer de entrada na conexão
const HTTPRequest* Connection::request()
{
    if (!m_request)
    {
        m_request = new HTTPRequest("");
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

void Connection::updateLastActivity()
{
    m_timeOfLastActivity = time(NULL);
}
