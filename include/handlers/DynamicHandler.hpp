#ifndef DYNAMIC_HANDLER_HPP
#define DYNAMIC_HANDLER_HPP

#include "ARequestHandler.hpp"

class DynamicHandler : public ARequestHandler
{
public:
    DynamicHandler();
    void handle(HTTPRequest& req, HTTPResponse& res, const ConfigSpec& cfg);

private:
    void handleGet(HTTPRequest& req, HTTPResponse& res, const ConfigSpec& cfg);
    void handlePost(HTTPRequest& req, HTTPResponse& res, const ConfigSpec& cfg);
};

#endif // !DYNAMIC_HANDLER_HPP
