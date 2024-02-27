#include "RouteHandler.hpp"
#include <iostream>

RouteHandler::RouteHandler(ARequestHandler* next)
    : ARequestHandler(next)
{
}

// TODO: validar rota primeiro, depois o método
// caso erro, exceção
// NOT FOUND significa 'open falhou'
void RouteHandler::handle(const HTTPRequest& req, HTTPResponse& res, const ConfigSpec& cfg)
{
    if (_next == NULL)
    {
        return;
    }

    try
    {
        std::string path = req.path();
        if (path == "/")
        {
            _next->handle(req, res, cfg);
            return;
        }
        if (cfg.hasLocation(path))
        {
            _next->handle(req, res, cfg.getLocation(path));
            return;
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "RouteHandler: " << e.what() << std::endl;
    }
    // throw HTTPException(404);
}
