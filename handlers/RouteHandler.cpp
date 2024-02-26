#include <iostream>
#include "RouteHandler.hpp"

RouteHandler::RouteHandler(const ConfigSpec& cfg)
    : ARequestHandler(cfg)
{
}

void RouteHandler::handle(HTTPRequest& req, HTTPResponse& res)
{
    std::cout << "Route: " << req.path() << std::endl;

    std::string path = req.path();
    if (path != "/")
    {
        if (_cfg.hasLocation(path))
        {
            _next->setContext(_cfg.getLocation(path));
        }
    }

    // int statusCode = res.getStatus();
    // if (_cfg.hasErrorPage(statusCode))
    // {
    //     // TODO: open error_page
    //     res.setBody(_cfg.getErrorPage(statusCode));
    // }
    // else
    // {
    //     // TODO: set default error_page
    //     std::cout << "Not implemented." << std::endl;
    // }
    if (_next != NULL)
    {
        _next->handle(req, res);
    }
}
