#ifndef HTTP_CONTANTS_HPP
#define HTTP_CONTANTS_HPP

#include <map>
#include <string>

const std::string CRLF = "\r\n";
const std::string DELIMITER = CRLF + CRLF;

typedef std::map<std::string, std::string> Headers;
typedef std::map<std::string, std::string> BodyContent;

enum Status
{
    CONTINUE = 100,
    OK = 200,
    MOVED_PERMANENTLY = 301,
    BAD_REQUEST = 400,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,
    REQUEST_ENTITY_TOO_LARGE = 413,
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
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
    TRANSFER_CONTROL,
    CONTENT_LENGTH,
    CHUNKED,
    FINISH
};

enum BodyType
{
    URLEncoded,
    FormData,
};

const std::string& statusText(int code);

#endif // !HTTP_CONTANTS_HPP
