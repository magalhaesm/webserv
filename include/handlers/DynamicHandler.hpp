#ifndef DYNAMIC_HANDLER_HPP
#define DYNAMIC_HANDLER_HPP

#include "ARequestHandler.hpp"

class DynamicHandler : public ARequestHandler
{
public:
    DynamicHandler();
    void handle(Request& req, Response& res, const ConfigSpec& cfg);

private:
    void handleGet(Request& req, Response& res, const ConfigSpec& cfg);
    void handlePost(Request& req, Response& res, const ConfigSpec& cfg);
};

#endif // !DYNAMIC_HANDLER_HPP
