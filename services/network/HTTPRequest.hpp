#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <map>
#include <string>
#include <stdexcept>

class Connection;

// - Representa solicitações HTTP.
// - Encapsula informações, como método HTTP, URI, cabeçalhos e corpo.
class HTTPRequest
{
public:
    HTTPRequest(const std::string& input);
    ~HTTPRequest();

    const std::string& method() const;
    const std::string& path() const;
    const std::string& getHeader(const std::string& field) const;

    void validate() const;

private:
    typedef std::map<std::string, std::string> Headers;

    Connection* m_conn;
    std::string m_method;
    std::string m_path;
    Headers m_headers;
    const std::string m_empty;

    void parseRequestLine(std::istringstream& stream);
    void parseHeaders(std::istringstream& stream);
    void parseBody(std::istringstream& stream);
};

std::ostream& operator<<(std::ostream& os, const HTTPRequest& request);

class BadRequestException : public std::runtime_error
{
public:
    BadRequestException(const std::string& message);
};

#endif // !HTTP_REQUEST_HPP
