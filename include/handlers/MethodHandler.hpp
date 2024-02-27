#ifndef METHOD_HANDLER_HPP
#define METHOD_HANDLER_HPP

#include "ARequestHandler.hpp"

class MethodHandler : public ARequestHandler
{
public:
    MethodHandler(ARequestHandler* next);
    void handle(const HTTPRequest& req, HTTPResponse& res, const ConfigSpec& cfg);
};

#endif // !METHOD_HANDLER_HPP
