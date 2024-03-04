#include <cstdio>
#include <fstream>

#include "Body.hpp"
#include "strings.hpp"
#include "filesystem.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "HTTPConstants.hpp"
#include "StaticHandler.hpp"

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

static std::string genAutoIndex(const std::string& root, const std::string& location);

StaticHandler::StaticHandler()
{
}

void StaticHandler::handle(Request& req, Response& res, const ConfigSpec& cfg)
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
        sendErrorPage(405, res, cfg);
    }

    if (_next)
    {
        _next->handle(req, res, cfg);
    }
}

void StaticHandler::handleGet(Request& req, Response& res, const ConfigSpec& cfg)
{
    if (ft::isDir(req.fullPath()))
    {
        if (sendIndex(req, res, cfg))
        {
            return;
        }
        if (sendAutoIndex(req, res, cfg))
        {
            return;
        }
        sendErrorPage(403, res, cfg);
        return;
    }

    std::ifstream resource(req.fullPath().c_str());
    if (resource.is_open())
    {
        res.setStatus(200);
        res.setBody(resource);
        return;
    }
    sendErrorPage(404, res, cfg);
}

void StaticHandler::handlePost(Request& req, Response& res, const ConfigSpec& cfg)
{
    Body* body = req.body();
    switch (body->getType())
    {
    case FormData:
    {
        const std::string filename = body->get("filename");
        const std::string fullPath = req.fullPath() + "/" + filename;

        if (rename(filename.c_str(), fullPath.c_str()) == 0)
        {
            sendErrorPage(200, res, cfg);
            return;
        }
        break;
    }
    case URLEncoded:
        sendErrorPage(501, res, cfg);
        return;
    }
    sendErrorPage(404, res, cfg);
}

void StaticHandler::handleDelete(Request& req, Response& res, const ConfigSpec& cfg)
{
    if (ft::isDir(req.fullPath()))
    {
        sendErrorPage(403, res, cfg);
        return;
    }
    if (remove(req.fullPath().c_str()) != 0)
    {
        sendErrorPage(500, res, cfg);
        return;
    }
    sendErrorPage(200, res, cfg);
}

bool StaticHandler::sendIndex(Request& req, Response& res, const ConfigSpec& cfg)
{
    std::string index = req.fullPath() + '/' + cfg.getIndex();
    std::ifstream page(index.c_str());
    if (page.is_open())
    {
        res.setStatus(200);
        res.setBody(page);
        return true;
    }
    return false;
}

bool StaticHandler::sendAutoIndex(Request& req, Response& res, const ConfigSpec& cfg)
{
    if (!cfg.hasAutoIndex())
    {
        return false;
    }
    try
    {
        std::string autoindex = genAutoIndex(req.fullPath(), cfg.getLocation());
        res.setStatus(200);
        res.setBody(autoindex);
    }
    catch (const std::exception& e)
    {
        sendErrorPage(500, res, cfg);
    }
    return true;
}

std::string genAutoIndex(const std::string& path, const std::string& location)
{
    ft::Strings dir = ft::listDir(path);
    std::string listing;
    for (ft::Strings::iterator it = dir.begin(); it != dir.end(); ++it)
    {
        std::string tag = A_TAG;
        ft::replace(tag, "CONTENT", *it);
        listing.append(tag);
    }
    std::string page = AUTOINDEX_TEMPLATE;
    ft::replace(page, "TITLE", location);
    ft::replace(page, "LISTING", listing);
    return page;
}
