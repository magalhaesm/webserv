#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <string>

#include "HTTP.hpp"

class Connection;

// - Representa solicitações HTTP.
// - Encapsula informações, como método HTTP, URI, cabeçalhos e corpo.
class HTTPRequest
{
public:
    HTTPRequest(const std::string& input);
    ~HTTPRequest();

    static HTTPRequest createRequest(const std::string& input);
    const std::string& method() const;
    const std::string& path() const;
    const std::string& getHeader(const std::string& field) const;

    void validate() const;

private:
    Connection* m_conn;
    std::string m_method;
    std::string m_path;
    std::string m_version;
    std::string m_query;
    Headers m_headers;
    const std::string m_empty;

    void parseRequestLine(std::istringstream& stream);
    void parseHeaders(std::istringstream& stream);
    void parseBody(std::istringstream& stream);
};

std::ostream& operator<<(std::ostream& os, const HTTPRequest& request);

#endif // !HTTP_REQUEST_HPP
