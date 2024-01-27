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
#include "Connection.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

const int BACKLOG = 10;

static void fatalError(const std::string& errMsg);

Server::Server(const ConfigSpec& cfg)
    : m_name(cfg.getServerName())
    , m_host(cfg.getHostName())
    , m_port(cfg.getPort())
{
    m_socket = createSocket();
}

Server::~Server()
{
    close(m_socket);
}

void Server::handleRequest(const HTTPRequest* request, HTTPResponse* response)
{
    if (isCGIRequest(request))
    {
        cgiController.handleCGIRequest(request, response);
        return;
    }
    htmlController.handleHTMLRequest(request, response);
}

inline bool Server::isCGIRequest(const HTTPRequest* request)
{
    (void)request;
    return false;
}

bool Server::isHTMLRequest(const HTTPRequest* request)
{
    (void)request;
    return true;
}

void Server::listen()
{
    if (::listen(m_socket, BACKLOG) == -1)
    {
        fatalError("Error while listening");
    }
}

int Server::accept()
{
    return ::accept(m_socket, NULL, NULL);
}

int Server::getSocket() const
{
    return m_socket;
}

int Server::createSocket()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        fatalError("Error creating socket");
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port);
    addr.sin_addr.s_addr = INADDR_ANY;

    int yes = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
        fatalError("Error during socket initialization");
    }

    return fd;
}

void fatalError(const std::string& errMsg)
{
    std::cerr << errMsg << ": " << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
}
