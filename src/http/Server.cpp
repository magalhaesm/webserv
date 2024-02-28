#include <cerrno>
#include <string>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "Server.hpp"
#include "AccessControlHandler.hpp"
#include "ConfigSpec.hpp"
#include "Connection.hpp"
#include "DynamicHandler.hpp"
#include "StaticHandler.hpp"
#include "LocationHandler.hpp"

const int BACKLOG = SOMAXCONN;

static void fatalError(const std::string& errMsg);

Server::Server(const ConfigSpec& cfg)
    : _cfg(cfg)
    , _name(cfg.getServerName())
    , _port(cfg.getPort())
{
    _socket = createSocket();
    setupHandlers();
}

Server::~Server()
{
    for (size_t idx = 0; idx < _handlers.size(); ++idx)
    {
        delete _handlers[idx];
    }
    close(_socket);
}

void Server::handleRequest(HTTPRequest& req, HTTPResponse& res)
{
    _initHandler->handle(req, res, _cfg);
}

void Server::listen()
{
    if (::listen(_socket, BACKLOG) == -1)
    {
        fatalError("Error while listening");
    }
}

int Server::accept()
{
    return ::accept(_socket, NULL, NULL);
}

int Server::getSocket() const
{
    return _socket;
}

int Server::getClientMaxBodySize() const
{
    return _cfg.getClientBodySize();
}

int Server::createSocket()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        fatalError("Error creating socket");
    }

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(_port);
    addr.sin_addr.s_addr = INADDR_ANY;

    int yes = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
        fatalError("Error during socket initialization");
    }

    return fd;
}

void Server::setupHandlers()
{
    ARequestHandler* location = new LocationHandler();
    _handlers.push_back(location);

    ARequestHandler* access = new AccessControlHandler();
    _handlers.push_back(access);

    ARequestHandler* dynamicContent = new DynamicHandler();
    _handlers.push_back(dynamicContent);

    ARequestHandler* staticContent = new StaticHandler();
    _handlers.push_back(staticContent);

    _initHandler = location;
    location->setNext(access);
    access->setNext(dynamicContent);
    dynamicContent->setNext(staticContent);
}

void fatalError(const std::string& errMsg)
{
    std::cerr << errMsg << ": " << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
}
