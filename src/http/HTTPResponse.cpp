#include <fstream>
#include <sstream>

#include "strings.hpp"
#include "Connection.hpp"
#include "HTTPResponse.hpp"

HTTPResponse::HTTPResponse()
{
}

HTTPResponse::~HTTPResponse()
{
}

void HTTPResponse::setStatus(int status)
{
    _statusCode = status;
}

void HTTPResponse::setHeader(const std::string& field, const std::string& value)
{
    _headers[field] = value;
}

void HTTPResponse::setHeader(const std::string& field, int value)
{
    _headers[field] = ft::itoa(value);
}

void HTTPResponse::setBody(const std::string& body)
{
    _body = body;
    setHeader("Content-Length", _body.size());
}

void HTTPResponse::setBody(const std::ifstream& body)
{
    std::stringstream buffer;

    buffer << body.rdbuf();
    _body = buffer.str();
    setHeader("Content-Length", _body.size());
}

int HTTPResponse::getStatus()
{
    return _statusCode;
}

const std::string& HTTPResponse::getHeader(const std::string& field)
{
    Headers::const_iterator it = _headers.find(field);
    if (it != _headers.end())
    {
        return it->second;
    }
    return _empty;
}

const std::string& HTTPResponse::HTTPResponse::toString()
{
    if (_text.empty())
    {
        std::ostringstream oss;

        oss << "HTTP/1.1 " << _statusCode << " ";
        oss << statusText(_statusCode) << CRLF;

        Headers::const_iterator it;
        for (it = _headers.begin(); it != _headers.end(); ++it)
        {
            oss << it->first << ": " << it->second << CRLF;
        }

        oss << CRLF << _body;
        _text = oss.str();
    }
    return _text;
}
