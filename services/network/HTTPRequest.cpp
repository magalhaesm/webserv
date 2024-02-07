#include <sstream>

#include "HTTPRequest.hpp"

HTTPRequest::HTTPRequest(const std::string& input)
{
    std::istringstream stream(input);
    parseRequestLine(stream);
    parseHeaders(stream);
}

HTTPRequest::~HTTPRequest()
{
}

void HTTPRequest::validate() const
{
    if (m_method.empty() || m_path.empty() || m_headers.empty())
    {
        throw BadRequestException("Malformed request");
    }
}

const std::string& HTTPRequest::method() const
{
    return m_method;
}

const std::string& HTTPRequest::path() const
{
    return m_path;
}

const std::string& HTTPRequest::getHeader(const std::string& field) const
{
    Headers::const_iterator it = m_headers.find(toLower(field));
    if (it != m_headers.end())
    {
        return it->second;
    }
    return m_empty;
}

void HTTPRequest::parseRequestLine(std::istringstream& stream)
{
    stream >> m_method >> m_path >> m_version;
}

void HTTPRequest::parseHeaders(std::istringstream& stream)
{
    std::string line;

    while (std::getline(stream, line) && !line.empty())
    {
        if (line[line.length() - 1] == '\r')
        {
            line.resize(line.length() - 1);
        }

        size_t colonPos = line.find(": ");
        if (colonPos != std::string::npos)
        {
            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 2);
            m_headers[toLower(key)] = value;
        }
    }
}
