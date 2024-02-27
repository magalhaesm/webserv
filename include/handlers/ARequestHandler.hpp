#ifndef AREQUEST_HANDLER_HPP
#define AREQUEST_HANDLER_HPP

#include "ConfigSpec.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

class ARequestHandler
{
public:
    ARequestHandler(ARequestHandler* next);
    virtual ~ARequestHandler();

    virtual void handle(const HTTPRequest& req, HTTPResponse& res, const ConfigSpec& cfg) = 0;

protected:
    ARequestHandler* _next;
};

#endif // !AREQUEST_HANDLER_HPP
