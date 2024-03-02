#include <fstream>

#include "strings.hpp"
#include "filesystem.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
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
static bool sendIndex(const std::string& path, HTTPResponse& res, const ConfigSpec& cfg);

StaticHandler::StaticHandler()
{
}

void StaticHandler::handle(HTTPRequest& req, HTTPResponse& res, const ConfigSpec& cfg)
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

void StaticHandler::handleGet(HTTPRequest& req, HTTPResponse& res, const ConfigSpec& cfg)
{
    std::string path = cfg.getRoot() + req.path();
    path = ft::strClean(path, '/');

    if (ft::isDir(path))
    {
        if (sendIndex(path, res, cfg))
        {
            return;
        }
        if (cfg.hasAutoindex())
        {
            try
            {
                std::string autoindex = genAutoIndex(path, req.realPath());
                res.setStatus(200);
                res.setBody(autoindex);
                return;
            }
            catch (const std::exception& e)
            {
                sendErrorPage(500, res, cfg);
                return;
            }
        }
        sendErrorPage(404, res, cfg);
        return;
    }

    std::ifstream resource(path.c_str());
    if (!resource.is_open())
    {
        sendErrorPage(404, res, cfg);
        return;
    }
    res.setStatus(200);
    res.setBody(resource);
}

void StaticHandler::handlePost(HTTPRequest&, HTTPResponse& res, const ConfigSpec& cfg)
{
    sendErrorPage(501, res, cfg);
}

void StaticHandler::handleDelete(HTTPRequest&, HTTPResponse& res, const ConfigSpec& cfg)
{
    sendErrorPage(501, res, cfg);
}

bool sendIndex(const std::string& path, HTTPResponse& res, const ConfigSpec& cfg)
{
    std::string index = path + '/' + cfg.getIndex();
    std::ifstream resource(index.c_str());
    if (resource.is_open())
    {
        res.setStatus(200);
        res.setBody(resource);
        return true;
    }
    return false;
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
