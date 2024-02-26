#ifndef AREQUEST_HANDLER_HPP
#define AREQUEST_HANDLER_HPP

#include "ConfigSpec.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

class ARequestHandler
{
public:
    ARequestHandler(const ConfigSpec& cfg);
    virtual ~ARequestHandler();
    virtual void handle(HTTPRequest& req, HTTPResponse& res) = 0;
    void setNext(ARequestHandler* handler);
    void setContext(const ConfigSpec& cfg);

protected:
    const ConfigSpec& _cfg;
    ARequestHandler* _next;
};

#endif // !AREQUEST_HANDLER_HPP
