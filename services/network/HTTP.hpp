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

    enum ParsingState
    {
        HEADERS,
        BODY,
        CONTENT_ENCODING,
        CONTENT_LENGTH,
        CHUNKED,
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
        ParsingState state;
        size_t cursor;

        Message();
        void clear();
    };

    class HTTPException;
};

const std::string CRLF = "\r\n";
const std::string HEADER_END = CRLF + CRLF;

std::string toLower(const std::string& input);

class BadRequestException : public std::runtime_error
{
public:
    BadRequestException(const std::string& message);
};

#endif // !HTTP_HPP
