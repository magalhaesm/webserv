#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <string>

#include "HTTPConstants.hpp"

class Response
{
public:
    Response();
    ~Response();

    void setStatus(int status);
    void setHeader(const std::string& field, const std::string& value);
    void setHeader(const std::string& field, int value);
    void setBody(const std::string& body);
    void setBody(const std::string& body, bool cgi);
    void setBody(const std::ifstream& body);

    int getStatus();
    const std::string& getHeader(const std::string& field);
    const std::string& toString();

private:
    int _statusCode;
    Headers _headers;
    std::string _body;
    std::string _text;
    const std::string _empty;
};

#endif // !HTTP_RESPONSE_HPP
