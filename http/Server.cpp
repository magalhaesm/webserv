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

#include "RedirectHandler.hpp"
#include "ErrorPageHandler.hpp"
#include "RouteHandler.hpp"

const int BACKLOG = 10;

const std::string ERROR_MESSAGE_TEMPLATE =

    "<!DOCTYPE html>\n"
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
    : m_cfg(cfg)
    , m_name(cfg.getServerName())
    , m_port(cfg.getPort())
{
    m_socket = createSocket();
    setupHandlers();
}

Server::~Server()
{
    for (size_t idx = 0; idx < m_handlers.size(); ++idx)
    {
        delete m_handlers[idx];
    }
    close(m_socket);
}

void Server::handleRequest(HTTPRequest& req, HTTPResponse& res)
{
    // if (cgiController.isCGI(req))
    // {
    //     cgiController.handleCGIRequest(req, res);
    //     return;
    // }
    m_handler->handle(req, res);
    htmlController.handleHTMLRequest(req, res);
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

    struct sockaddr_in addr = {};
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

void Server::setupHandlers()
{
    ARequestHandler* router = new RouteHandler(m_cfg);
    m_handlers.push_back(router);

    ARequestHandler* redirect = new RedirectHandler(m_cfg);
    m_handlers.push_back(redirect);

    ARequestHandler* erroPage = new ErrorPageHandler(m_cfg);
    m_handlers.push_back(erroPage);

    m_handler = router;
    router->setNext(redirect);
    redirect->setNext(erroPage);
}

void fatalError(const std::string& errMsg)
{
    std::cerr << errMsg << ": " << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
}
