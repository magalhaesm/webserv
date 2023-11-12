#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <map>
#include <string>

class Connection;

// - Representa respostas HTTP.
// - Encapsula informações, como status code e corpo.
class HTTPResponse
{
public:
    HTTPResponse(Connection* conn);
    ~HTTPResponse();

    void setStatus(int status);
    void set(const std::string& field, const std::string& value);
    void setBody(const std::string& body);
    std::string toString() const;

private:
    typedef std::map<std::string, std::string> Header;

    Connection* m_conn;
    int m_statusCode;
    Header m_header;
    std::string m_body;
};

#endif // !HTTP_RESPONSE_HPP
