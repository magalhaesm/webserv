#include "AccessControlHandler.hpp"
#include "strings.hpp"

AccessControlHandler::AccessControlHandler()
{
}

void AccessControlHandler::handle(HTTPRequest& req, HTTPResponse& res, const ConfigSpec& cfg)
{
    if (!cfg.allow(req.methodText()))
    {
        sendErrorPage(405, res, cfg);
        return;
    }

    std::string filename = (req.path() == "/") ? cfg.getIndex() : req.path();
    filename = cfg.getRoot() + filename;
    ft::replace(filename, "//", "/");
    req.setPath(filename);

    if (_next)
    {
        _next->handle(req, res, cfg);
    }
}
