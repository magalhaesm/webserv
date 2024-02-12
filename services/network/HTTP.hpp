#ifndef HTTP_HPP
#define HTTP_HPP

#include <map>
#include <string>
#include <stdexcept>

namespace http
{
    typedef std::map<std::string, std::string> Headers;

    enum Status
    {
        OK = 200

    };

    enum Method
    {
        GET,
        POST,
        DELETE,
        UNKNOWN
    };

    enum State
    {
        START,
        FINISHED
    };

    struct Message
    {
        Method method;
        std::string version;
        std::string path;
        std::string query;
        Headers headers;
        std::string body;
        State state;
        int cursor;
    };

    class HTTPException;
};

const std::string CRLF = "\r\n";

std::string toLower(const std::string& input);

class BadRequestException : public std::runtime_error
{
public:
    BadRequestException(const std::string& message);
};

#endif // !HTTP_HPP
