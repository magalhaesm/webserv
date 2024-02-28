#include <fstream>

#include "strings.hpp"
#include "filesystem.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "StaticHandler.hpp"

static std::string generateAutoIndex(const std::string& dirPath);

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
    if (ft::isDir(req.path().c_str()))
    {
        if (!cfg.hasAutoindex())
        {
            sendErrorPage(404, res, cfg);
            return;
        }
        try
        {
            std::string autoindex = generateAutoIndex(req.path());
            res.setStatus(200);
            res.setBody(autoindex);
        }
        catch (const std::exception& e)
        {
            sendErrorPage(500, res, cfg);
        }
        return;
    }

    std::ifstream resource(req.path().c_str());
    if (!resource.is_open())
    {
        sendErrorPage(404, res, cfg);
        return;
    }
    res.setStatus(200);
    res.setBody(resource);
}

void StaticHandler::handlePost(HTTPRequest&, HTTPResponse&, const ConfigSpec&)
{
}

void StaticHandler::handleDelete(HTTPRequest&, HTTPResponse&, const ConfigSpec&)
{
}

std::string generateAutoIndex(const std::string& dirPath)
{
    std::string content;
    ft::Strings dir = ft::listDir(dirPath);
    for (ft::Strings::iterator it = dir.begin(); it != dir.end(); ++it)
    {
        std::string li = LI_TAG;
        ft::replace(li, "REPLACE", *it);
        content.append(li);
    }
    std::string page = AUTOINDEX_TEMPLATE;
    ft::replace(page, "CONTENT", content);
    return page;
}
