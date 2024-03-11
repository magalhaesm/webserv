#include <fstream>

#include "Logger.hpp"
#include "filesystem.hpp"
#include "IndexHandler.hpp"
#include "InternalErrorException.hpp"

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

bool fileExists(const std::string& filename);
static std::string generateAutoIndex(const Request& req);

IndexContentHandler::IndexContentHandler()
{
}

void IndexContentHandler::handle(Request& req, Response& res, const ConfigSpec& cfg)
{
    if (ft::isDir(req.realPath()))
    {
        std::string index = req.realPath() + '/' + cfg.getIndex();
        if (cfg.hasIndex() && fileExists(index))
        {
            req.setRealPath(index);
            _next->handle(req, res, cfg);
            return;
        }

        if (cfg.hasAutoIndex())
        {
            try
            {
                std::string autoindex = generateAutoIndex(req);
                res.setStatus(OK);
                res.setBody(autoindex);
            }
            catch (const InternalErrorException& e)
            {
                Logger::log(e.what());
                sendStatusPage(INTERNAL_SERVER_ERROR, res, cfg);
            }
        }
    }

    if (_next)
    {
        _next->handle(req, res, cfg);
    }
    return;
}

bool fileExists(const std::string& filename)
{
    std::ifstream file(filename.c_str());
    return file.good();
}

std::string generateAutoIndex(const Request& req)
{
    ft::Strings dir = ft::scanDir(req.realPath());

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
