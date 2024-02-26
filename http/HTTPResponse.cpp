#include <sstream>

#include "definitions.hpp"
#include "Connection.hpp"
#include "HTTPResponse.hpp"

const std::string& httpStatusCode(int code);

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

int HTTPResponse::getStatus()
{
    return m_statusCode;
}

const std::string& HTTPResponse::getHeader(const std::string& field)
{
    Headers::const_iterator it = m_headers.find(field);
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
        oss << httpStatusCode(m_statusCode) << CRLF;

        Headers::const_iterator it;
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

const std::string& httpStatusCode(int code)
{
    static std::map<int, std::string> status;

    status[100] = "Continue";
    status[200] = "OK";
    status[201] = "Created";
    status[301] = "Moved Permanently";
    status[400] = "Bad Request";
    status[404] = "Not Found";
    status[405] = "Method not allowed";
    status[500] = "Internal Server Error";
    status[501] = "Not Implemented";
    status[503] = "Service Unavailable";
    status[505] = "HTTP Version Not Supported";

    return status[code];
}
