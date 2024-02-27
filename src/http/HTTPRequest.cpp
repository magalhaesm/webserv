#include "strings.hpp"
#include "HTTPRequest.hpp"

HTTPRequest::HTTPRequest(Message& message)
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

Method HTTPRequest::method() const
{
    return m_msg.method;
}

const std::string& HTTPRequest::path() const
{
    return m_msg.path;
}

const std::string& HTTPRequest::query() const
{
    return m_msg.query;
}

const std::string& HTTPRequest::getHeader(const std::string& field) const
{
    Headers::const_iterator it = m_msg.headers.find(ft::toLower(field));
    if (it != m_msg.headers.end())
    {
        return it->second;
    }
    return m_empty;
}
