#ifndef DYNAMIC_CONTENT_HANDLER_HPP
#define DYNAMIC_CONTENT_HANDLER_HPP

#include "ARequestHandler.hpp"

class DynamicContentHandler : public ARequestHandler
{
public:
    DynamicContentHandler();
    void handle(Request& req, Response& res, const ConfigSpec& cfg);

private:
    void handleGet(Request& req, Response& res, const ConfigSpec& cfg);
    void handlePost(Request& req, Response& res, const ConfigSpec& cfg);
};

#endif // !DYNAMIC_CONTENT_HANDLER_HPP
