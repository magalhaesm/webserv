#ifndef ROUTE_HANDLER_HPP
#define ROUTE_HANDLER_HPP

#include "ARequestHandler.hpp"

class RouteHandler : public ARequestHandler
{
public:
    RouteHandler(const ConfigSpec& cfg);
    void handle(HTTPRequest& req, HTTPResponse& res);
};

#endif // !ROUTE_HANDLER_HPP
