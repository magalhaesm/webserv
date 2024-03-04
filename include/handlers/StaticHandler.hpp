#ifndef STATIC_HANDLER_HPP
#define STATIC_HANDLER_HPP

#include "ARequestHandler.hpp"

class StaticHandler : public ARequestHandler
{
public:
    StaticHandler();
    void handle(Request& req, Response& res, const ConfigSpec& cfg);

private:
    void handleGet(Request& req, Response& res, const ConfigSpec& cfg);
    void handlePost(Request& req, Response& res, const ConfigSpec& cfg);
    void handleDelete(Request& req, Response& res, const ConfigSpec& cfg);

    bool sendIndex(Request& req, Response& res, const ConfigSpec& cfg);
    bool sendAutoIndex(Request& req, Response& res, const ConfigSpec& cfg);
};

#endif // !STATIC_HANDLER_HPP
