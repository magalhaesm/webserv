#include "HTTPConstants.hpp"
#include "AccessControlHandler.hpp"

AccessControlHandler::AccessControlHandler()
{
}

void AccessControlHandler::handle(Request& req, Response& res, const ConfigSpec& cfg)
{
    if (!cfg.isMethodAllowed(req.methodText()))
    {
        sendStatusPage(METHOD_NOT_ALLOWED, res, cfg);
        return;
    }
    if (cfg.hasRedirect())
    {
        Redirect redir = cfg.getRedirect();
        res.setHeader("Location", redir.url);
        sendStatusPage(redir.code, res, cfg);
        return;
    }
    if (_next)
    {
        _next->handle(req, res, cfg);
    }
}
