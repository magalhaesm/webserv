#include "HTTPRequest.hpp"

HTTPRequest::HTTPRequest(const http::Message& message)
    : m_msg(message)
{
}

HTTPRequest::HTTPRequest(const HTTPRequest& rhs)
    : m_msg(rhs.m_msg)
{
}

HTTPRequest& HTTPRequest::operator=(const HTTPRequest&)
{
    return *this;
}

HTTPRequest::~HTTPRequest()
{
}

http::Method HTTPRequest::method() const
{
    return m_msg.method;
}

const std::string& HTTPRequest::path() const
{
    return m_msg.path;
}

const std::string& HTTPRequest::getHeader(const std::string& field) const
{
    http::Headers::const_iterator it = m_msg.headers.find(toLower(field));
    if (it != m_msg.headers.end())
    {
        return it->second;
    }
    return m_empty;
}
