#ifndef HTTP_PARSER_HPP
#define HTTP_PARSER_HPP

#include <string>

#include "Message.hpp"

class HTTPParser
{
public:
    static bool parseHeader(std::string& raw, Message& msg);
    static bool parseBody(std::string& raw, Message& msg, int maxBodySize);
};

#endif // !HTTP_PARSER_HPP
