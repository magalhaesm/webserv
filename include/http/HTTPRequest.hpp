#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <string>

#include "Message.hpp"

class HTTPRequest
{
public:
    HTTPRequest(Message& message);
    HTTPRequest(const HTTPRequest& rhs);
    HTTPRequest& operator=(const HTTPRequest& rhs);
    ~HTTPRequest();

    Method method() const;
    const std::string& path() const;
    const std::string& query() const;
    const std::string& getHeader(const std::string& field) const;

private:
    Message& m_msg;
    const std::string m_empty;
};

std::ostream& operator<<(std::ostream& os, const HTTPRequest& request);

#endif // !HTTP_REQUEST_HPP
