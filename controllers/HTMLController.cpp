#include "HTMLController.hpp"

#include <iostream>

// TODO: refactor
// inspiração: https://drogon.org/

// maxBody
void HTMLController::handleHTMLRequest(const HTTPRequest& request, HTTPResponse& response)
{
    // TODO: Lógica para manipular solicitações HTML
    // 1. Este servidor aceita este método? Not Implemented
    // 2. Esta rota é atendida por este servidor? Not Found

    std::string method = request.method();

    if (method == "GET")
    {
        handleGetRequest(request, response);
    }
    else if (method == "POST")
    {
        handlePostRequest(request, response);
    }
    else if (method == "DELETE")
    {
        handleDeleteRequest(request, response);
    }
    else
    {
        response.setStatus(405);
        response.setHeader("Content-Length", "52");
        response.setBody("<html><body><h1>Method not allowed</h1></body></html>");
    }
}

void HTMLController::handleGetRequest(const HTTPRequest& request, HTTPResponse& response)
{
    std::cout << "\nmethod: " << request.method() << std::endl;

    std::cout << "Host: " << request.getHeader("Host") << std::endl;
    std::cout << "User-Agent: " << request.getHeader("User-Agent") << std::endl;
    std::cout << "Connection: " << request.getHeader("Connection") << std::endl;

    response.setStatus(200);
    response.setHeader("Content-Type", "text/html");
    response.setHeader("Server", "Webserv");
    // response.setHeader("Connection", "close");
    response.setHeader("Connection", "keep-alive");
    response.setHeader("Content-Length", "65");
    response.setBody("<!DOCTYPE><html><body><h1>HTMLController here!</h1></body></html>");
}

void HTMLController::handlePostRequest(const HTTPRequest& request, HTTPResponse& response)
{
    std::cout << "method: " << request.method() << std::endl;

    response.setStatus(200);
    response.setHeader("Content-Type", "text/html");
    response.setHeader("Server", "Webserv");
    response.setHeader("Connection", "keep-alive");
    response.setHeader("Content-Length", "65");
    std::string body = "<!DOCTYPE><html><body><h1>HTMLController here!</h1></body></html>";
    response.setBody(body);
}

void HTMLController::handleDeleteRequest(const HTTPRequest& request, HTTPResponse& response)
{
    std::cout << "method: " << request.method() << std::endl;

    response.setStatus(200);
    response.setHeader("Content-Type", "text/html");
    response.setBody("<html><body><h1>HTMLController here!</h1></body></html>");
}
