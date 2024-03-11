#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <string>

#include "Message.hpp"

class Request
{
public:
    Request(Message& message);
    Request(const Request& rhs);
    Request& operator=(const Request& rhs);
    ~Request();

    Method method() const;
    const std::string methodText() const;
    const std::string& path() const;
    const std::string& query() const;
    const std::string& getHeader(const std::string& field) const;
    int body() const;
    void setRealPath(const std::string& path);
    const std::string& realPath() const;
    int error() const;

private:
    Message& _msg;
    std::string _realPath;
    const std::string _empty;
};

std::ostream& operator<<(std::ostream& os, const Request& request);

#endif // !HTTP_REQUEST_HPP
