#include "AccessControlHandler.hpp"

AccessControlHandler::AccessControlHandler()
{
}

void AccessControlHandler::handle(HTTPRequest& req, HTTPResponse& res, const ConfigSpec& cfg)
{
    if (cfg.allow(req.methodText()) == false)
    {
        sendErrorPage(405, res, cfg);
        return;
    }

    if (req.path() == "/")
    {
        req.setPath(cfg.getRoot() + cfg.getIndex());
    }
    else
    {
        req.setPath(cfg.getRoot() + req.path());
    }

    if (_next)
    {
        _next->handle(req, res, cfg);
    }
}
