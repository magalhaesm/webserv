#include <fstream>

#include "Request.hpp"
#include "Response.hpp"
#include "filesystem.hpp"
#include "HTTPConstants.hpp"
#include "StaticContentHandler.hpp"

StaticContentHandler::StaticContentHandler()
{
}

void StaticContentHandler::handle(Request& req, Response& res, const ConfigSpec& cfg)
{
    switch (req.method())
    {
    case GET:
        handleGet(req, res, cfg);
        break;
    case POST:
        handlePost(req, res, cfg);
        break;
    case DELETE:
        handleDelete(req, res, cfg);
        break;
    default:
        sendStatusPage(METHOD_NOT_ALLOWED, res, cfg);
    }

    if (_next)
    {
        _next->handle(req, res, cfg);
    }
}

void StaticContentHandler::handleGet(Request& req, Response& res, const ConfigSpec& cfg)
{
    std::ifstream resource(req.realPath().c_str());
    if (resource.is_open())
    {
        res.setStatus(OK);
        res.setBody(resource);
        return;
    }
    sendStatusPage(NOT_FOUND, res, cfg);
}

void StaticContentHandler::handlePost(Request&, Response& res, const ConfigSpec& cfg)
{
    sendStatusPage(NOT_IMPLEMENTED, res, cfg);
}

void StaticContentHandler::handleDelete(Request& req, Response& res, const ConfigSpec& cfg)
{
    if (ft::isDir(req.realPath()))
    {
        sendStatusPage(FORBIDDEN, res, cfg);
        return;
    }
    if (remove(req.realPath().c_str()) != 0)
    {
        sendStatusPage(INTERNAL_SERVER_ERROR, res, cfg);
        return;
    }
    sendStatusPage(OK, res, cfg);
}
