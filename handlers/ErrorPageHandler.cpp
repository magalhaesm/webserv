#include <iostream>

#include "ARequestHandler.hpp"
#include "ErrorPageHandler.hpp"

ErrorPageHandler::ErrorPageHandler(const ConfigSpec& cfg)
    : ARequestHandler(cfg)
{
}

void ErrorPageHandler::handle(HTTPRequest& req, HTTPResponse& res)
{
    std::cout << "ErrorPageHandler" << std::endl;
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
