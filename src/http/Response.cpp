#include <fstream>
#include <sstream>
#include <string>

#include "strings.hpp"
#include "Connection.hpp"
#include "Response.hpp"
#include "HTTPConstants.hpp"

Response::Response()
    : _statusCode(0)
{
}

Response::~Response()
{
}

void Response::setStatus(int status)
{
    _statusCode = status;
}

void Response::setHeader(const std::string& field, const std::string& value)
{
    _headers[field] = value;
}

void Response::setHeader(const std::string& field, int value)
{
    _headers[field] = ft::itoa(value);
}

void Response::setBody(const std::string& body)
{
    _body = body;
    setHeader("Content-Length", _body.size());
}

void Response::setBody(const std::string& body, bool)
{
    size_t headerEnd = body.find(CRLF);
    if (headerEnd == std::string::npos)
    {
        setBody(body);
        return;
    }

    std::string line;
    std::istringstream iss(body.substr(0, headerEnd));
    while (std::getline(iss, line))
    {
        size_t colonPos = line.find(": ");
        if (colonPos != std::string::npos)
        {
            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 2);
            if (ft::toLower(key) == "status")
            {
                setStatus(std::atoi(value.c_str()));
                continue;
            }
            setHeader(key, value);
        }
    }
    setBody(body.c_str() + headerEnd + CRLF.length());
}

void Response::setBody(const std::ifstream& body)
{
    std::stringstream buffer;

    buffer << body.rdbuf();
    _body = buffer.str();
    setHeader("Content-Length", _body.size());
}

int Response::getStatus()
{
    return _statusCode;
}

const std::string& Response::getHeader(const std::string& field)
{
    Headers::const_iterator it = _headers.find(field);
    if (it != _headers.end())
    {
        return it->second;
    }
    return _empty;
}

const std::string& Response::Response::toString()
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
