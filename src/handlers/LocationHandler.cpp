#include "LocationHandler.hpp"

LocationHandler::LocationHandler()
{
}

void LocationHandler::handle(HTTPRequest& req, HTTPResponse& res, const ConfigSpec& cfg)
{
    if (req.error())
    {
        sendErrorPage(req.error(), res, cfg);
        return;
    }

    ConfigSpec context = cfg;
    std::string path = req.path();
    if (cfg.hasLocation(path))
    {
        context = cfg.getLocation(path);
    }

    if (_next)
    {
        _next->handle(req, res, context);
    }
}
