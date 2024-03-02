#include "LocationHandler.hpp"

LocationHandler::LocationHandler()
{
}

void LocationHandler::handle(HTTPRequest& req, HTTPResponse& res, const ConfigSpec& cfg)
{
    if (!_next)
    {
        return;
    }

    if (req.error())
    {
        sendErrorPage(req.error(), res, cfg);
        return;
    }

    std::string location = cfg.match(req.path());
    if (!location.empty())
    {
        _next->handle(req, res, cfg.getContext(location));
        return;
    }
    _next->handle(req, res, cfg);
}
