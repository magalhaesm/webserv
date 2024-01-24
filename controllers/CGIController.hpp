#ifndef CGI_CONTROLLER_HPP
#define CGI_CONTROLLER_HPP

#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

class CGIController
{
public:
    void handleCGIRequest(const HTTPRequest* request, HTTPResponse* response);

private:
    void handleGetRequest(const HTTPRequest* request, HTTPResponse* response);
    void handlePostRequest(const HTTPRequest* request, HTTPResponse* response);
};

#endif // !CGI_CONTROLLER_HPP
