#include <cstdio>
#include <fstream>

#include "Body.hpp"
#include "strings.hpp"
#include "filesystem.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "HTTPConstants.hpp"
#include "StaticContentHandler.hpp"

const std::string AUTOINDEX_TEMPLATE =

    "<!DOCTYPE html>\n"
    "<html lang=\"en\">\n"
    "<head>\n"
    "  <title>Index of TITLE</title>\n"
    "</head>\n"
    "<body>\n"
    "  <h1>Index of TITLE</h1>\n"
    "  <hr>\n"
    "  <pre>\n"
    "    <a href=\"../\">../</a>\n"
    "LISTING"
    "  </pre>\n"
    "  <hr>\n"
    "</body>\n"
    "</html>\n";

const std::string A_TAG = "    <a href=\"CONTENT\">CONTENT</a>\n";

static std::string generateAutoIndex(const Request& req);

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
    if (ft::isDir(req.realPath()))
    {
        if (sendIndex(req, res, cfg))
        {
            return;
        }
        if (sendAutoIndex(req, res, cfg))
        {
            return;
        }
        sendStatusPage(FORBIDDEN, res, cfg);
        return;
    }

    std::ifstream resource(req.realPath().c_str());
    if (resource.is_open())
    {
        res.setStatus(OK);
        res.setBody(resource);
        return;
    }
    sendStatusPage(NOT_FOUND, res, cfg);
}

void StaticContentHandler::handlePost(Request& req, Response& res, const ConfigSpec& cfg)
{
    Body* body = req.body();
    switch (body->getType())
    {
    case FormData:
    {
        const std::string filename = body->get("filename");
        const std::string fullPath = req.realPath() + "/" + filename;

        if (rename(filename.c_str(), fullPath.c_str()) == 0)
        {
            sendStatusPage(OK, res, cfg);
            return;
        }
        break;
    }
    case URLEncoded:
        sendStatusPage(NOT_IMPLEMENTED, res, cfg);
        return;
    }
    sendStatusPage(NOT_FOUND, res, cfg);
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

bool StaticContentHandler::sendIndex(Request& req, Response& res, const ConfigSpec& cfg)
{
    std::string index = req.realPath() + '/' + cfg.getIndex();
    std::ifstream page(index.c_str());
    if (page.is_open())
    {
        res.setStatus(OK);
        res.setBody(page);
        return true;
    }
    return false;
}

bool StaticContentHandler::sendAutoIndex(Request& req, Response& res, const ConfigSpec& cfg)
{
    if (!cfg.hasAutoIndex())
    {
        return false;
    }
    try
    {
        std::string autoindex = generateAutoIndex(req);
        res.setStatus(OK);
        res.setBody(autoindex);
    }
    catch (...)
    {
        sendStatusPage(INTERNAL_SERVER_ERROR, res, cfg);
    }
    return true;
}

std::string generateAutoIndex(const Request& req)
{
    ft::Strings dir = ft::listDir(req.realPath());

    std::string listing;
    for (ft::Strings::iterator it = dir.begin(); it != dir.end(); ++it)
    {
        std::string tag = A_TAG;
        ft::replace(tag, "CONTENT", *it);
        listing.append(tag);
    }

    std::string page = AUTOINDEX_TEMPLATE;
    ft::replace(page, "TITLE", req.path());
    ft::replace(page, "LISTING", listing);
    return page;
}
