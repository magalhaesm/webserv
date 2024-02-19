#include "CGIController.hpp"

void CGIController::handleCGIRequest(const HTTPRequest& request, HTTPResponse& response)
{
    // TODO: Lógica para manipular solicitações CGI
    switch (request.method())
    {
    case GET:
        handleGetRequest(request, response);
        break;
    case POST:
        handlePostRequest(request, response);
        break;
    default:
        response.setStatus(405);
        response.setHeader("Content-Length", "52");
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
    (void)request;
    response.setStatus(200);
    response.setHeader("Content-Type", "text/html");
    response.setBody("<html><body><h1>CGIController here!</h1></body></html>");
}

void CGIController::handlePostRequest(const HTTPRequest& request, HTTPResponse& response)
{
    (void)request;
    response.setStatus(200);
    response.setHeader("Content-Type", "text/html");
    response.setBody("<html><body><h1>CGIController here!</h1></body></html>");
}
