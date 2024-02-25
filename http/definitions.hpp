#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

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
    Chunked
};

#endif // !DEFINITIONS_HPP
