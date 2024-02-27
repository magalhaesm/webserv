#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <string>

#include "HTTPConstants.hpp"

class HTTPResponse
{
public:
    HTTPResponse();
    ~HTTPResponse();

    void setStatus(int status);
    void setHeader(const std::string& field, const std::string& value);
    void setBody(const std::string& body);

    int getStatus();

    const std::string& getHeader(const std::string& field);
    const std::string& toString();
    bool isKeepAlive();

private:
    int _statusCode;
    Headers _headers;
    std::string _body;
    std::string _text;

    const std::string m_empty;
};

#endif // !HTTP_RESPONSE_HPP
