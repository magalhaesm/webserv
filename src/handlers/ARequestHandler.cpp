#include <fstream>

#include "strings.hpp"
#include "ARequestHandler.hpp"

const std::string ERROR_PAGE_TEMPLATE =

    "<!DOCTYPE html>\n"
    "<html lang=\"en\">\n"
    "  <head>\n"
    "    <title>ERROR_MESSAGE</title>\n"
    "  </head>\n"
    "  <body>\n"
    "    <center>\n"
    "      <h1>ERROR_MESSAGE</h1>\n"
    "    </center>\n"
    "    <hr>\n"
    "    <center>webserv</center>\n"
    "  </body>\n"
    "</html>";

ARequestHandler ::ARequestHandler()
    : _next(NULL)
{
}

void ARequestHandler::setNext(ARequestHandler* next)
{
    _next = next;
}

void ARequestHandler::sendErrorPage(int code, HTTPResponse& res, const ConfigSpec& cfg)
{
    if (cfg.hasErrorPage(code))
    {
        std::string pageName = cfg.getRoot() + cfg.getErrorPage(code);
        std::ifstream page(pageName.c_str());
        if (page)
        {
            res.setStatus(code);
            res.setBody(page);
            return;
        }
    }
    sendDefaultErrorPage(code, res);
}

void ARequestHandler::sendDefaultErrorPage(int code, HTTPResponse& res)
{
    std::string page = ERROR_PAGE_TEMPLATE;
    std::string search = "ERROR_MESSAGE";
    std::string replace = ft::itoa(code) + " " + statusText(code);
    ft::replace(page, search, replace);

    res.setStatus(code);
    res.setBody(page);
}

ARequestHandler::~ARequestHandler()
{
}
