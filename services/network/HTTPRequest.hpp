#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <string>

#include "HTTP.hpp"

class HTTPRequest
{
public:
    HTTPRequest(const http::Message& message);
    HTTPRequest(const HTTPRequest& rhs);
    HTTPRequest& operator=(const HTTPRequest& rhs);
    ~HTTPRequest();

    const std::string& method() const;
    const std::string& path() const;
    const std::string& getHeader(const std::string& field) const;

private:
    const http::Message& m_msg; // TODO: talvez deva ser um ponteiro
    const std::string m_empty;

    void parseRequestLine(std::istringstream& stream);
    void parseHeaders(std::istringstream& stream);
    void parseBody(std::istringstream& stream);
};

std::ostream& operator<<(std::ostream& os, const HTTPRequest& request);

#endif // !HTTP_REQUEST_HPP
