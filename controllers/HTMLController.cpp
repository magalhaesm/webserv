#include "HTMLController.hpp"

#include <iostream>

void HTMLController::handleHTMLRequest(const HTTPRequest* request, HTTPResponse* response)
{
    // TODO: Lógica para manipular solicitações HTML
    // localizar recurso do modelo
    // atualizar view

    std::string method = request->method();

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
        response->setStatus(405);
        response->setBody("<html><body><h1>Method not allowed</h1></body></html>");
    }
}

void HTMLController::handleGetRequest(const HTTPRequest* request, HTTPResponse* response)
{
    std::cout << "method: " << request->method() << std::endl;

    response->setStatus(200);
    response->setHeader("Content-Type", "text/html");
    response->setBody("<html><body><h1>HTMLController here!</h1></body></html>");
}

void HTMLController::handlePostRequest(const HTTPRequest* request, HTTPResponse* response)
{
    std::cout << "method: " << request->method() << std::endl;

    response->setStatus(200);
    response->setHeader("Content-Type", "text/html");
    response->setBody("<html><body><h1>HTMLController here!</h1></body></html>");
}

void HTMLController::handleDeleteRequest(const HTTPRequest* request, HTTPResponse* response)
{
    std::cout << "method: " << request->method() << std::endl;

    response->setStatus(200);
    response->setHeader("Content-Type", "text/html");
    response->setBody("<html><body><h1>HTMLController here!</h1></body></html>");
}
