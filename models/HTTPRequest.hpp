#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <map>
#include <string>
#include <vector>

class Connection;

// - Representa solicitações HTTP.
// - Encapsula informações, como método HTTP, URI, cabeçalhos e corpo.
class HTTPRequest
{
public:
    HTTPRequest(Connection* conn);
    ~HTTPRequest();

    const std::string& method() const;
    const std::string& URL() const;
    std::string get(const std::string& field) const;

private:
    typedef std::map<std::string, std::string> Header;

    Connection* m_conn;
    std::string m_method;
    std::string m_url;
    Header m_header;

    void parseRequestLine(std::istringstream& stream);
    void parseHeaders(std::istringstream& stream);
    void parseBody(std::istringstream& stream);
};

#endif // !HTTP_REQUEST_HPP
