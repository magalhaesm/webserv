#include <cerrno>
#include <string>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "Server.hpp"
#include "Connection.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

const int BACKLOG = 10;

const std::string ERROR_MESSAGE_TEMPLATE =

    "<!DOCTYPE HTML>\n"
    "<html lang=\"en\">\n"
    "  <head>\n"
    "    <title>{ERROR_MESSAGE}</title>\n"
    "  </head>\n"
    "  <body>\n"
    "    <center>\n"
    "      <h1>{ERROR_MESSAGE}</h1>\n"
    "    </center>\n"
    "    <hr>\n"
    "    <center>webserv</center>\n"
    "  </body>\n"
    "</html>";

static void fatalError(const std::string& errMsg);

Server::Server(const ConfigSpec& cfg)
    : m_name(cfg.getServerName())
    , m_port(cfg.getPort())
    , m_parser(cfg.getBodySizeLimit())
{
    m_socket = createSocket();
}

Server::~Server()
{
    close(m_socket);
}

void Server::handleRequest(const HTTPRequest& request, HTTPResponse& response)
{
    if (cgiController.isCGI(request))
    {
        cgiController.handleCGIRequest(request, response);
        return;
    }
    htmlController.handleHTMLRequest(request, response);
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

HTTPParser& Server::parser()
{
    return m_parser;
}

int Server::createSocket()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        fatalError("Error creating socket");
    }

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
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
