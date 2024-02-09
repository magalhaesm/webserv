#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <string>

#include "HTTP.hpp"

class Connection;

class HTTPResponse
{
public:
    HTTPResponse(Connection* conn);
    ~HTTPResponse();

    void setStatus(int status);
    void setHeader(const std::string& field, const std::string& value);
    void setBody(const std::string& body);

    bool isPersistent();
    const std::string& getHeader(const std::string& field);

    const std::string& toString();

private:
    Connection* m_conn;
    int m_statusCode;
    http::Headers m_header;
    std::string m_body;
    std::string m_text;

    const std::string m_empty;
};

#endif // !HTTP_RESPONSE_HPP
