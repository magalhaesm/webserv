#include "HTTPConstants.hpp"
#include "DynamicContentHandler.hpp"

DynamicContentHandler::DynamicContentHandler()
{
}

void DynamicContentHandler::handle(Request& req, Response& res, const ConfigSpec& cfg)
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
        sendStatusPage(METHOD_NOT_ALLOWED, res, cfg);
    }
}

void DynamicContentHandler::handleGet(Request&, Response& res, const ConfigSpec& cfg)
{
    sendStatusPage(NOT_IMPLEMENTED, res, cfg);
}

void DynamicContentHandler::handlePost(Request&, Response& res, const ConfigSpec& cfg)
{
    sendStatusPage(NOT_IMPLEMENTED, res, cfg);
}
