#ifndef AREQUEST_HANDLER_HPP
#define AREQUEST_HANDLER_HPP

#include "ConfigSpec.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

class ARequestHandler
{
public:
    ARequestHandler();
    virtual ~ARequestHandler();

    virtual void handle(HTTPRequest& req, HTTPResponse& res, const ConfigSpec& cfg) = 0;
    void setNext(ARequestHandler* next);

protected:
    ARequestHandler* _next;

    void sendErrorPage(int code, HTTPResponse& res, const ConfigSpec& cfg);

private:
    void sendDefaultErrorPage(int code, HTTPResponse& res);
};

#endif // !AREQUEST_HANDLER_HPP
