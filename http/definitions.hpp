#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#include <map>
#include <string>

const std::string CRLF = "\r\n";
const std::string DELIMITER = CRLF + CRLF;

typedef std::map<std::string, std::string> Headers;
typedef std::map<std::string, std::string> BodyContent;

const std::string ERROR_MESSAGE_TEMPLATE =

    "<!DOCTYPE html>\n"
    "<html lang=\"en\">\n"
    "  <head>\n"
    "    <title>{ERROR_MESSAGE}</title>\n"
    "  </head>\n"
    "  <body>\n"
    "    <center>\n"
    "      <h1>{ERROR_MESSAGE}</h1>\n"
    "    </center>\n"
    "    <hr>\n"
    "    <center>webserv</center>\n"
    "  </body>\n"
    "</html>";

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
    BODY_TYPE,
    BODY_CONTENT,
    FINISH
};

enum BodyType
{
    URLEncoded,
    FormData,
};

#endif // !DEFINITIONS_HPP
