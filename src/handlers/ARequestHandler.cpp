#include <fstream>

#include "strings.hpp"
#include "ARequestHandler.hpp"

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
        std::string pageName = cfg.getErrorPage(code);
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
    std::string page = ERROR_MESSAGE_TEMPLATE;
    std::string search = "ERROR_MESSAGE";
    std::string replace = ft::itoa(code) + " " + statusText(code);
    ft::replace(page, search, replace);

    res.setStatus(code);
    res.setBody(page);
}

ARequestHandler::~ARequestHandler()
{
}
