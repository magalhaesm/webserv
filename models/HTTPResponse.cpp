#include <sstream>

#include "Connection.hpp"
#include "HTTPResponse.hpp"

const std::string& getStatusCode(int code);

HTTPResponse::HTTPResponse(Connection* conn)
    : m_conn(conn)
{
}

HTTPResponse::~HTTPResponse()
{
}

void HTTPResponse::setStatus(int status)
{
    m_statusCode = status;
}

void HTTPResponse::set(const std::string& field, const std::string& value)
{
    m_header[field] = value;
}

void HTTPResponse::setBody(const std::string& body)
{
    m_body = body;
}

std::string HTTPResponse::HTTPResponse::toString() const
{
    std::ostringstream oss;

    oss << "HTTP/1.1 " << m_statusCode << " ";
    oss << getStatusCode(m_statusCode) << "\r\n";
    for (Header::const_iterator it = m_header.begin(); it != m_header.end(); ++it)
    {
        oss << it->first << ": " << it->second << "\r\n";
    }
    oss << "\r\n" << m_body;
    return oss.str();
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
    statusCodes[505] = "HTTP Version Not Supported";

    return statusCodes[code];
}
