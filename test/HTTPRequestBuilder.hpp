#ifndef HTTP_REQUEST_BUILDER_HPP
#define HTTP_REQUEST_BUILDER_HPP

#include <map>
#include <sstream>

class HTTPRequestBuilder
{
public:
    HTTPRequestBuilder(const std::string& host = "localhost", const std::string& path = "/")
        : _host(host)
        , _path(path)
        , _contentType("application/x-www-form-urlencoded")
        , _body("")
        , _method("GET")
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
    }

    void setBody(const std::string& body)
    {
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
