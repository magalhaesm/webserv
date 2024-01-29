#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <map>
#include <string>

class Connection;

class HTTPResponse
{
public:
    HTTPResponse(Connection* conn);
    ~HTTPResponse();

    void setStatus(int status);
    void setHeader(const std::string& field, const std::string& value);
    void setBody(const std::string& body);

    const std::string& toString();

private:
    typedef std::map<std::string, std::string> Headers;

    Connection* m_conn;
    int m_statusCode;
    Headers m_header;
    std::string m_body;
    std::string m_text;
};

#endif // !HTTP_RESPONSE_HPP
