#include "strings.hpp"
#include "LocationHandler.hpp"

LocationHandler::LocationHandler()
{
}

void LocationHandler::handle(Request& req, Response& res, const ConfigSpec& cfg)
{
    if (!_next)
    {
        return;
    }

    if (req.error())
    {
        sendStatusPage(req.error(), res, cfg);
        return;
    }

    std::string realPath = cfg.getRoot() + req.path();
    req.setRealPath(ft::strClean(realPath, '/'));

    std::string location = cfg.match(req.path());
    if (!location.empty())
    {
        _next->handle(req, res, cfg.getContext(location));
        return;
    }
    _next->handle(req, res, cfg);
}
