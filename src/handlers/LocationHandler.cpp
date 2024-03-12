#include "strings.hpp"
#include "LocationHandler.hpp"

static void setRealPath(Request& req, const ConfigSpec& cfg);

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

    setRealPath(req, cfg);

    std::string location = cfg.match(req.path());
    if (!location.empty())
    {
        _next->handle(req, res, cfg.getContext(location));
        return;
    }
    _next->handle(req, res, cfg);
}

inline void setRealPath(Request& req, const ConfigSpec& cfg)
{
    std::string realPath = cfg.getRoot() + req.path();
    req.setRealPath(ft::strClean(realPath, '/'));
}
