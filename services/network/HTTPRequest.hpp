#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <string>

#include "HTTP.hpp"

class HTTPRequest
{
public:
    // HTTPRequest(const std::string& input);
    HTTPRequest(const http::Message& message);
    HTTPRequest(const HTTPRequest& rhs);
    HTTPRequest& operator=(const HTTPRequest& rhs);
    ~HTTPRequest();

    const std::string& method() const;
    const std::string& path() const;
    const std::string& getHeader(const std::string& field) const;

    void validate() const;

private:
    std::string m_method;
    std::string m_path;
    std::string m_version;
    std::string m_query;

    const http::Message& m_msg;
    http::Headers m_headers;

    const std::string m_empty;

    void parseRequestLine(std::istringstream& stream);
    void parseHeaders(std::istringstream& stream);
    void parseBody(std::istringstream& stream);
};

// class HTTPRequest
// {
// public:
//     HTTPRequest(http::Message* input);
//     HTTPRequest(const HTTPRequest&);
//     HTTPRequest& operator=(const HTTPRequest&);
//     ~HTTPRequest();
//
//     const std::string& method() const;
//     const std::string& path() const;
//     const std::string& getHeader(const std::string& field) const;
//
// private:
//     http::Headers m_headers;
//     http::Message* m_msg;
//
//     const std::string m_empty;
// };

std::ostream& operator<<(std::ostream& os, const HTTPRequest& request);

#endif // !HTTP_REQUEST_HPP
