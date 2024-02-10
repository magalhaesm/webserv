#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <string>

#include "HTTP.hpp"

class HTTPResponse
{
public:
    HTTPResponse();
    ~HTTPResponse();

    void setStatus(int status);
    void setHeader(const std::string& field, const std::string& value);
    void setBody(const std::string& body);

    const std::string& getHeader(const std::string& field);
    const std::string& toString();
    bool isKeepAlive();

private:
    int m_statusCode;
    http::Headers m_headers;
    std::string m_body;
    std::string m_text;

    const std::string m_empty;
};

#endif // !HTTP_RESPONSE_HPP
