#include "AccessControlHandler.hpp"

AccessControlHandler::AccessControlHandler()
{
}

void AccessControlHandler::handle(Request& req, Response& res, const ConfigSpec& cfg)
{
    if (!cfg.isMethodAllowed(req.methodText()))
    {
        sendStatusPage(405, res, cfg);
        return;
    }

    if (_next)
    {
        _next->handle(req, res, cfg);
    }
}
