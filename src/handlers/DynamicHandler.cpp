#include "DynamicHandler.hpp"

DynamicHandler::DynamicHandler()
{
}

void DynamicHandler::handle(Request& req, Response& res, const ConfigSpec& cfg)
{
    if (!cfg.hasCGI())
    {
        _next->handle(req, res, cfg);
        return;
    }

    switch (req.method())
    {
    case GET:
        handleGet(req, res, cfg);
        break;
    case POST:
        handlePost(req, res, cfg);
        break;
    default:
        sendStatusPage(405, res, cfg);
    }
}

void DynamicHandler::handleGet(Request&, Response&, const ConfigSpec&)
{
}

void DynamicHandler::handlePost(Request&, Response&, const ConfigSpec&)
{
}
