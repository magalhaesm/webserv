#ifndef ERRORPAGE_HANDLER_HPP
#define ERRORPAGE_HANDLER_HPP

#include "ARequestHandler.hpp"

class ErrorPageHandler : public ARequestHandler
{
public:
    ErrorPageHandler(const ConfigSpec& cfg);
    void handle(HTTPRequest& req, HTTPResponse& res);
};

#endif // !ERRORPAGE_HANDLER_HPP
