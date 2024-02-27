#ifndef ROUTE_HANDLER_HPP
#define ROUTE_HANDLER_HPP

#include "ARequestHandler.hpp"

class RouteHandler : public ARequestHandler
{
public:
    RouteHandler(ARequestHandler* next);
    void handle(const HTTPRequest& req, HTTPResponse& res, const ConfigSpec& cfg);
};

#endif // !ROUTE_HANDLER_HPP
