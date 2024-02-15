#ifndef HTTP_PARSER_HPP
#define HTTP_PARSER_HPP

#include "HTTP.hpp"

class HTTPParser
{
public:
    static bool parseRequest(const std::string& raw, http::Message& msg);
};

#endif // !HTTP_PARSER_HPP
