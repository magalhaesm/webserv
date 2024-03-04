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

static std::string generateAutoIndex(const Request& req);

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
        sendStatusPage(405, res, cfg);
    }

    if (_next)
    {
        _next->handle(req, res, cfg);
    }
}

void StaticHandler::handleGet(Request& req, Response& res, const ConfigSpec& cfg)
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
        sendStatusPage(403, res, cfg);
        return;
    }

    std::ifstream resource(req.realPath().c_str());
    if (resource.is_open())
    {
        res.setStatus(200);
        res.setBody(resource);
        return;
    }
    sendStatusPage(404, res, cfg);
}

void StaticHandler::handlePost(Request& req, Response& res, const ConfigSpec& cfg)
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
            sendStatusPage(200, res, cfg);
            return;
        }
        break;
    }
    case URLEncoded:
        sendStatusPage(501, res, cfg);
        return;
    }
    sendStatusPage(404, res, cfg);
}

void StaticHandler::handleDelete(Request& req, Response& res, const ConfigSpec& cfg)
{
    if (ft::isDir(req.realPath()))
    {
        sendStatusPage(403, res, cfg);
        return;
    }
    if (remove(req.realPath().c_str()) != 0)
    {
        sendStatusPage(500, res, cfg);
        return;
    }
    sendStatusPage(200, res, cfg);
}

bool StaticHandler::sendIndex(Request& req, Response& res, const ConfigSpec& cfg)
{
    std::string index = req.realPath() + '/' + cfg.getIndex();
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
        std::string autoindex = generateAutoIndex(req);
        res.setStatus(200);
        res.setBody(autoindex);
    }
    catch (const std::exception&)
    {
        sendStatusPage(500, res, cfg);
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
