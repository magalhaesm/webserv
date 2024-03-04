#include "strings.hpp"
#include "HTTPRequest.hpp"

HTTPRequest::HTTPRequest(Message& message)
    : _msg(message)
{
}

HTTPRequest::HTTPRequest(const HTTPRequest& rhs)
    : _msg(rhs._msg)
{
}

HTTPRequest& HTTPRequest::operator=(const HTTPRequest&)
{
    return *this;
}

HTTPRequest::~HTTPRequest()
{
}

Method HTTPRequest::method() const
{
    return _msg.method;
}

const std::string HTTPRequest::methodText() const
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

const std::string& HTTPRequest::path() const
{
    return _msg.path;
}

void HTTPRequest::setFullPath(const std::string& path)
{
    _fullPath = path;
}

const std::string& HTTPRequest::fullPath() const
{
    return _fullPath;
}

const std::string& HTTPRequest::query() const
{
    return _msg.query;
}

const std::string& HTTPRequest::getHeader(const std::string& field) const
{
    Headers::const_iterator it = _msg.headers.find(ft::toLower(field));
    if (it != _msg.headers.end())
    {
        return it->second;
    }
    return _empty;
}

Body* HTTPRequest::body() const
{
    return _msg.body;
}

int HTTPRequest::error() const
{
    return _msg.error;
}
