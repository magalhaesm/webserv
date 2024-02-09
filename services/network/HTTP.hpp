#ifndef HTTP_HPP
#define HTTP_HPP

#include <map>
#include <string>
#include <stdexcept>

namespace http
{
    typedef std::map<std::string, std::string> Headers;

    enum status
    {
        OK = 200

    };

    enum method
    {
        GET,
        POST,
        DELETE,
        UNKNOWN
    };

    struct Message
    {
        // http::method m_method;
        std::string method;
        std::string version;
        std::string path;
        std::string query;
        Headers headers;
        std::string body;

    public:
        Message();
        Message(const Message& rhs);
        Message& operator=(const Message& rhs);
        ~Message();
    };

    class HTTPException;
};

const std::string CRLF("\r\n");

std::string toLower(const std::string& input);

class BadRequestException : public std::runtime_error
{
public:
    BadRequestException(const std::string& message);
};

#endif // !HTTP_HPP
