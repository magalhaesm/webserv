#ifndef HTML_CONTROLLER_HPP
#define HTML_CONTROLLER_HPP

#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

class HTMLController
{
public:
    void handleHTMLRequest(const HTTPRequest& request, HTTPResponse& response);

private:
    void handleGetRequest(const HTTPRequest& request, HTTPResponse& response);
    void handlePostRequest(const HTTPRequest& request, HTTPResponse& response);
    void handleDeleteRequest(const HTTPRequest& request, HTTPResponse& response);
};

#endif // !HTML_CONTROLLER_HPP
