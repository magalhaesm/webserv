#include <sstream>

#include "Connection.hpp"
#include "HTTPResponse.hpp"

const std::string& getStatusCode(int code);

HTTPResponse::HTTPResponse()
{
}

HTTPResponse::~HTTPResponse()
{
}

void HTTPResponse::setStatus(int status)
{
    m_statusCode = status;
}

void HTTPResponse::setHeader(const std::string& field, const std::string& value)
{
    m_headers[field] = value;
}

void HTTPResponse::setBody(const std::string& body)
{
    m_body = body;
}

const std::string& HTTPResponse::getHeader(const std::string& field)
{
    http::Headers::const_iterator it = m_headers.find(field);
    if (it != m_headers.end())
    {
        return it->second;
    }
    return m_empty;
}

const std::string& HTTPResponse::HTTPResponse::toString()
{
    if (m_text.empty())
    {
        std::ostringstream oss;

        oss << "HTTP/1.1 " << m_statusCode << " ";
        oss << getStatusCode(m_statusCode) << CRLF;

        http::Headers::const_iterator it;
        for (it = m_headers.begin(); it != m_headers.end(); ++it)
        {
            oss << it->first << ": " << it->second << CRLF;
        }

        oss << CRLF << m_body;
        m_text = oss.str();
    }
    return m_text;
}

bool HTTPResponse::isKeepAlive()
{
    return getHeader("Connection") != "close";
}

const std::string& getStatusCode(int code)
{
    static std::map<int, std::string> statusCodes;

    statusCodes[100] = "Continue";
    statusCodes[200] = "OK";
    statusCodes[201] = "Created";
    statusCodes[301] = "Moved Permanently";
    statusCodes[400] = "Bad Request";
    statusCodes[404] = "Not Found";
    statusCodes[405] = "Method not allowed";
    statusCodes[500] = "Internal Server Error";
    statusCodes[501] = "Not Implemented";
    statusCodes[503] = "Service Unavailable";
    statusCodes[505] = "HTTP Version Not Supported";

    return statusCodes[code];
}
