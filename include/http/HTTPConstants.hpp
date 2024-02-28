#ifndef HTTP_CONTANTS_HPP
#define HTTP_CONTANTS_HPP

#include <map>
#include <string>

const std::string CRLF = "\r\n";
const std::string DELIMITER = CRLF + CRLF;

typedef std::map<std::string, std::string> Headers;
typedef std::map<std::string, std::string> BodyContent;

// TODO: rename ERROR_PAGE_TEMPLATE
const std::string ERROR_MESSAGE_TEMPLATE =

    "<!DOCTYPE html>\n"
    "<html lang=\"en\">\n"
    "  <head>\n"
    "    <title>ERROR_MESSAGE</title>\n"
    "  </head>\n"
    "  <body>\n"
    "    <center>\n"
    "      <h1>ERROR_MESSAGE</h1>\n"
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
