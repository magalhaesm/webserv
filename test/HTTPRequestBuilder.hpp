#ifndef HTTP_REQUEST_BUILDER_HPP
#define HTTP_REQUEST_BUILDER_HPP

#include <map>
#include <sstream>

#include "strings.hpp"

class HTTPRequestBuilder
{
public:
    HTTPRequestBuilder(const std::string& host = "localhost", const std::string& path = "/")
        : _host(host)
        , _path(path)
    {
    }

    void addHeader(const std::string& key, const std::string& value)
    {
        _headers[key] = value;
    }

    std::string getRequest() const
    {
        std::ostringstream oss;
        oss << _method << " " << _path << " HTTP/1.1\r\n";
        oss << "Host: " << _host << "\r\n";

        std::map<std::string, std::string>::const_iterator it;
        for (it = _headers.begin(); it != _headers.end(); ++it)
        {
            oss << it->first << ": " << it->second << "\r\n";
        }
        if (!_body.empty())
        {
            oss << "Content-Type: " << _contentType << "\r\n";
            oss << "Content-Length: " << _body.length() << "\r\n";
        }
        oss << "\r\n";
        oss << _body;
        return oss.str();
    }

    void setHost(const std::string& host)
    {
        _host = host;
    }

    void setPath(const std::string& path)
    {
        _path = path;
    }

    void setContentType(const std::string& contentType)
    {
        _contentType = contentType;

        if (_contentType == "multipart/form-data")
        {
            _contentType += "; boundary=---------------------------974767299852498929531610575";
        }
    }

    void setBody(const std::string& body)
    {
        _body = body;
    }

    void setFileToSend(const std::string& filename, const std::string& content)
    {
        std::string body =
            "-----------------------------974767299852498929531610575\r\n"
            "Content-Disposition: form-data; name=\"file\"; filename=\"FILE.txt\"\r\n"
            "Content-Type: text/plain\r\n\r\n"
            "(file contents here)\r\n"
            "-----------------------------974767299852498929531610575--\r\n";

        ft::replace(body, "FILE.txt", filename);
        ft::replace(body, "(file contents here)", content);

        _body = body;
    }

    void setMethod(const std::string& method)
    {
        _method = method;
    }

private:
    std::string _host;
    std::string _path;
    std::string _contentType;
    std::string _body;
    std::string _method;
    std::map<std::string, std::string> _headers;
};

#endif // !HTTP_REQUEST_BUILDER_HPP
