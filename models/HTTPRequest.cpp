#include <sstream>

#include "HTTPRequest.hpp"
#include "Connection.hpp"

HTTPRequest::HTTPRequest(Connection* conn)
    : m_conn(conn)
{
    // TODO: handle erros
    std::istringstream stream(conn->read());
    parseRequestLine(stream);
    parseHeaders(stream);
}

HTTPRequest::~HTTPRequest()
{
}

const std::string& HTTPRequest::method() const
{
    return m_method;
}

const std::string& HTTPRequest::URL() const
{
    return m_url;
}

std::string HTTPRequest::get(const std::string& field) const
{
    Header::const_iterator it = m_header.find(field);
    if (it != m_header.end())
    {
        return it->second;
    }
    return "";
}

void HTTPRequest::parseRequestLine(std::istringstream& stream)
{
    stream >> m_method >> m_url;
}

void HTTPRequest::parseHeaders(std::istringstream& stream)
{
    std::string line;

    while (std::getline(stream, line) && !line.empty())
    {
        size_t colonPos = line.find(": ");
        if (colonPos != std::string::npos)
        {
            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 2);
            m_header[key] = value;
        }
    }
}
