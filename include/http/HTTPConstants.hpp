#ifndef HTTP_CONTANTS_HPP
#define HTTP_CONTANTS_HPP

#include <map>
#include <string>

const std::string CRLF = "\r\n";
const std::string DELIMITER = CRLF + CRLF;

typedef std::map<std::string, std::string> Headers;
typedef std::map<std::string, std::string> BodyContent;

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
    BODY_TYPE,
    BODY_CONTENT,
    FINISH
};

enum BodyType
{
    URLEncoded,
    FormData,
};

const std::string& statusText(int code);

#endif // !HTTP_CONTANTS_HPP
