#ifndef AREQUEST_HANDLER_HPP
#define AREQUEST_HANDLER_HPP

#include "ConfigSpec.hpp"
#include "Request.hpp"
#include "Response.hpp"

class ARequestHandler
{
public:
    ARequestHandler();
    virtual ~ARequestHandler();

    virtual void handle(Request& req, Response& res, const ConfigSpec& cfg) = 0;
    void setNext(ARequestHandler* next);

protected:
    ARequestHandler* _next;

    void sendStatusPage(int code, Response& res, const ConfigSpec& cfg);
    bool fileExists(const std::string& filename);

private:
    void sendDefaultErrorPage(int code, Response& res);
};

#endif // !AREQUEST_HANDLER_HPP
