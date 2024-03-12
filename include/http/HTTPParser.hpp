#ifndef HTTP_PARSER_HPP
#define HTTP_PARSER_HPP

#include <string>

#include "Message.hpp"

class HTTPParser
{
public:
    static bool parseRequest(std::string& raw, Message& msg, size_t maxBodySize = 0);
};

#endif // !HTTP_PARSER_HPP
