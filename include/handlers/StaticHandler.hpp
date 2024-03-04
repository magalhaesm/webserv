#ifndef STATIC_HANDLER_HPP
#define STATIC_HANDLER_HPP

#include "ARequestHandler.hpp"

class StaticHandler : public ARequestHandler
{
public:
    StaticHandler();
    void handle(HTTPRequest& req, HTTPResponse& res, const ConfigSpec& cfg);

private:
    void handleGet(HTTPRequest& req, HTTPResponse& res, const ConfigSpec& cfg);
    void handlePost(HTTPRequest& req, HTTPResponse& res, const ConfigSpec& cfg);
    void handleDelete(HTTPRequest& req, HTTPResponse& res, const ConfigSpec& cfg);

    bool sendIndex(HTTPRequest& req, HTTPResponse& res, const ConfigSpec& cfg);
    bool sendAutoIndex(HTTPRequest& req, HTTPResponse& res, const ConfigSpec& cfg);
};

#endif // !STATIC_HANDLER_HPP
