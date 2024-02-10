#ifndef HTTP_PARSER_HPP
#define HTTP_PARSER_HPP

#include <sstream>

#include "HTTPRequest.hpp"

class HTTPParser
{
public:
    HTTPParser();
    HTTPParser(unsigned int bodySizeLimitMB = 32);
    HTTPParser(const HTTPParser& rhs);
    HTTPParser& operator=(const HTTPParser& rhs);
    ~HTTPParser();

    const HTTPRequest& newHTTPRequest();
    bool parseRequest(const std::string& buffer);

    void parseRequestLine(std::istringstream& stream);
    void parseHeaders(std::istringstream& stream);

private:
    http::Message m_msg;
    HTTPRequest m_request;
    unsigned int m_bodySizeInBytes;
};
#endif // !HTTP_PARSER_HPP
