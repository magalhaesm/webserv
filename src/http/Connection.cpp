#include <cstdlib>
#include <unistd.h>
#include <strings.h>
#include <sys/socket.h>

#include "Connection.hpp"
#include "HTTPParser.hpp"
#include "EventListener.hpp"

const int BUFSIZE = 8192;

Connection::Connection(EventListener* listener, Server* server)
    : _server(server)
    , _listener(listener)
    , _lastActivityTime(time(NULL))
{
    _clientSocket = server->accept();
    _maxBodySize = _server->getClientMaxBodySize();
}

Connection::~Connection()
{
    ::close(_clientSocket);
}

bool Connection::read()
{
    char buffer[BUFSIZE];
    ssize_t bytesRead = ::recv(_clientSocket, buffer, BUFSIZE, 0);
    if (bytesRead <= 0)
    {
        return this->close();
    }

    this->updateLastActivityTime();
    _raw.append(buffer, bytesRead);

    if (HTTPParser::parseRequest(_raw, _msg, _maxBodySize))
    {
        this->processRequest();
        return true;
    }
    return false;
}

bool Connection::write()
{
    if (_raw.empty())
    {
        return true;
    }

    size_t bufsiz = std::min<int>(_raw.size(), BUFSIZE);
    ssize_t bytesWritten = ::send(_clientSocket, _raw.c_str(), bufsiz, 0);
    if (bytesWritten <= 0)
    {
        return this->close();
    }

    this->updateLastActivityTime();
    _raw.erase(0, bytesWritten);

    if (_raw.empty())
    {
        return true;
    }
    return false;
}

inline void Connection::processRequest()
{
    Request request(_msg);
    Response response;
    _server->handleRequest(request, response);
    this->send(response.toString());
}

inline void Connection::send(const std::string& response)
{
    _raw = response;
    clear(_msg);
}

bool Connection::close()
{
    _listener->close(this);
    return true;
}

int Connection::getID() const
{
    return _clientSocket;
}

std::time_t Connection::getLastActivityTime() const
{
    return _lastActivityTime;
}

void Connection::updateLastActivityTime()
{
    _lastActivityTime = time(NULL);
}
