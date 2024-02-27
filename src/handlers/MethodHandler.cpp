#include <iostream>
#include "definitions.hpp"

#include "MethodHandler.hpp"

MethodHandler::MethodHandler(ARequestHandler* next)
    : ARequestHandler(next)
{
}

void MethodHandler::handle(const HTTPRequest& req, HTTPResponse& res, const ConfigSpec& cfg)
{
    switch (req.method())
    {
    case GET:
    case POST:
    case DELETE:
        break;
    default:

        break;
    }
    (void)req;
    (void)res;
    (void)cfg;

    std::cout << "Autoindex: " << cfg.hasAutoindex() << std::endl;
    throw std::runtime_error("MethodHandler");
    // if (_next == NULL)
    // {
    //     return;
    // }
}
