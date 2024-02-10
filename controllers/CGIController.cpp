#include <iostream>

#include "CGIController.hpp"

void CGIController::handleCGIRequest(const HTTPRequest& request, HTTPResponse& response)
{
    // TODO: Lógica para manipular solicitações CGI
    std::string method = request.method();

    if (method == "GET")
    {
        handleGetRequest(request, response);
    }
    else if (method == "POST")
    {
        handlePostRequest(request, response);
    }
    else
    {
        response.setStatus(405);
        response.setBody("<html><body><h1>Method not allowed</h1></body></html>");
    }
}

bool CGIController::isCGI(const HTTPRequest& request)
{
    (void)request;
    return false;
}

void CGIController::handleGetRequest(const HTTPRequest& request, HTTPResponse& response)
{
    std::cout << "method: " << request.method() << std::endl;
    // std::cout << "path: " << request.path() << std::endl;
    // std::cout << "Host: " << request.get("Host") << std::endl;
    // std::cout << "Content-Type: " << request.get("Content-Type") << std::endl;
    // std::cout << "User-Agent: " << request.get("User-Agent") << std::endl;

    response.setStatus(200);
    response.setHeader("Content-Type", "text/html");
    response.setBody("<html><body><h1>CGIController here!</h1></body></html>");
}

void CGIController::handlePostRequest(const HTTPRequest& request, HTTPResponse& response)
{
    std::cout << "method: " << request.method() << std::endl;

    response.setStatus(200);
    response.setHeader("Content-Type", "text/html");
    response.setBody("<html><body><h1>CGIController here!</h1></body></html>");
}
