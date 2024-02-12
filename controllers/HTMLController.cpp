#include "HTMLController.hpp"

#include <iostream>
#include "HTTP.hpp"

void HTMLController::handleHTMLRequest(const HTTPRequest& request, HTTPResponse& response)
{
    // TODO: Lógica para manipular solicitações HTML
    // 1. Este servidor aceita este método? Not Implemented
    // 2. Esta rota é atendida por este servidor? Not Found

    switch (request.method())
    {
    case http::GET:
        handleGetRequest(request, response);
        break;
    case http::POST:
        handlePostRequest(request, response);
        break;
    case http::DELETE:
        handleDeleteRequest(request, response);
        break;
    default:
        response.setStatus(405);
        response.setHeader("Content-Length", "52");
        response.setBody("<html><body><h1>Method not allowed</h1></body></html>");
    }
}

void HTMLController::handleGetRequest(const HTTPRequest& request, HTTPResponse& response)
{
    // std::cout << "\nmethod: " << request.method() << std::endl;
    std::cout << "Path: " << request.path() << std::endl;

    std::cout << "Host: " << request.getHeader("Host") << std::endl;
    std::cout << "User-Agent: " << request.getHeader("User-Agent") << std::endl;
    std::cout << "Connection: " << request.getHeader("Connection") << std::endl;

    response.setHeader("Content-Type", "text/html");
    response.setHeader("Server", "Webserv");
    // response.setHeader("Connection", "close");
    response.setHeader("Connection", "keep-alive");

    if (request.path() == "/favicon.ico")
    {
        response.setStatus(404);
        response.setHeader("Content-Length", "59");
        response.setBody("<!DOCTYPE html><html><body><h1>Not Found</h1></body></html>");
        return;
    }

    response.setStatus(200);
    response.setHeader("Content-Length", "69");
    response.setBody("<!DOCTYPE html><html><body><h1>HTMLController here</h1></body></html>");
}

void HTMLController::handlePostRequest(const HTTPRequest& request, HTTPResponse& response)
{
    (void)request;
    response.setStatus(200);
    response.setHeader("Content-Type", "text/html");
    response.setHeader("Server", "Webserv");
    response.setHeader("Connection", "keep-alive");
    response.setHeader("Content-Length", "65");
    std::string body = "<!DOCTYPE HTML><html><body><h1>HTMLController here!</h1></body></html>";
    response.setBody(body);
}

void HTMLController::handleDeleteRequest(const HTTPRequest& request, HTTPResponse& response)
{
    std::cout << "method: " << request.method() << std::endl;

    response.setStatus(200);
    response.setHeader("Content-Type", "text/html");
    response.setBody("<html><body><h1>HTMLController here!</h1></body></html>");
}
