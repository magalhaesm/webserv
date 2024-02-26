// #include "Directives.hpp"
#include "RedirectHandler.hpp"
#include <iostream>

RedirectHandler::RedirectHandler(const ConfigSpec& cfg)
    : ARequestHandler(cfg)
{
}

void RedirectHandler::handle(HTTPRequest& req, HTTPResponse& res)
{
    std::cout << "RedirectHandler\n";

    // if (_cfg.hasRedirect())
    // {
    //     Redirect redir = _cfg.getRedirect();
    //     res.setStatus(redir.first);
    //     req.setPath(redir.second);
    // }

    if (_next != NULL)
    {
        _next->handle(req, res);
    }
}
