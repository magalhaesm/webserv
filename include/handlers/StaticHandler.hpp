#ifndef STATIC_HANDLER_HPP
#define STATIC_HANDLER_HPP

#include "ARequestHandler.hpp"

const std::string AUTOINDEX_TEMPLATE =

    "<!DOCTYPE html>\n"
    "<html lang=\"en\">\n"
    "<head>\n"
    "  <meta charset=\"UTF-8\">\n"
    "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
    "  <title>TITLE</title>\n"
    "</head>\n"
    "<body>\n"
    "  <h1>TITLE</h1>\n"
    "  <ul>\n"
    "CONTENT"
    "  </ul>\n"
    "</body>\n"
    "</html>\n";

const std::string LI_TAG = "    <li><a href=\"REPLACE\">REPLACE</a></li>\n";

class StaticHandler : public ARequestHandler
{
public:
    StaticHandler();
    void handle(HTTPRequest& req, HTTPResponse& res, const ConfigSpec& cfg);

private:
    void handleGet(HTTPRequest& req, HTTPResponse& res, const ConfigSpec& cfg);
    void handlePost(HTTPRequest& req, HTTPResponse& res, const ConfigSpec& cfg);
    void handleDelete(HTTPRequest& req, HTTPResponse& res, const ConfigSpec& cfg);
};

#endif // !STATIC_HANDLER_HPP
