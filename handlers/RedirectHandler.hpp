#ifndef REDIRECT_HANDLER_HPP
#define REDIRECT_HANDLER_HPP

#include "ARequestHandler.hpp"

class RedirectHandler : public ARequestHandler
{
public:
    RedirectHandler(const ConfigSpec& cfg);
    void handle(HTTPRequest& req, HTTPResponse& res);
};

#endif // !REDIRECT_HANDLER_HPP
