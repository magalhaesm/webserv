#include <unistd.h>

#include "strings.hpp"
#include "Request.hpp"

Request::Request(Message& message)
    : _msg(message)
{
}

Request::Request(const Request& rhs)
    : _msg(rhs._msg)
{
}

Request& Request::operator=(const Request&)
{
    return *this;
}

Request::~Request()
{
}

Method Request::method() const
{
    return _msg.method;
}

const std::string Request::methodText() const
{
    switch (_msg.method)
    {
    case GET:
        return "GET";
    case POST:
        return "POST";
    case DELETE:
        return "DELETE";
    case UNKNOWN:
        return "UNKNOWN";
    }
    return _empty;
}

const std::string& Request::path() const
{
    return _msg.path;
}

void Request::setRealPath(const std::string& path)
{
    _realPath = path;
}

const std::string& Request::realPath() const
{
    return _realPath;
}

const std::string& Request::query() const
{
    return _msg.query;
}

const std::string& Request::getHeader(const std::string& field) const
{
    Headers::const_iterator it = _msg.headers.find(ft::toLower(field));
    if (it != _msg.headers.end())
    {
        return it->second;
    }
    return _empty;
}

int Request::body() const
{
    return _msg.body;
}

std::string Request::bodyName() const
{
    return _msg.bodyFilename;
}

int Request::error() const
{
    return _msg.error;
}
