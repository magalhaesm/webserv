#include "DynamicHandler.hpp"

DynamicHandler::DynamicHandler()
{
}

void DynamicHandler::handle(HTTPRequest& req, HTTPResponse& res, const ConfigSpec& cfg)
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
        sendErrorPage(405, res, cfg);
    }
}

void DynamicHandler::handleGet(HTTPRequest&, HTTPResponse&, const ConfigSpec&)
{
}

void DynamicHandler::handlePost(HTTPRequest&, HTTPResponse&, const ConfigSpec&)
{
}
