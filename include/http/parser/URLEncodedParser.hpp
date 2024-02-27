#ifndef URLENCODED_PARSER_HPP
#define URLENCODED_PARSER_HPP

#include "ABodyParser.hpp"

class URLEncodedParser : public ABodyParser
{
public:
    URLEncodedParser(std::string& raw, Message& msg, size_t maxSize);
    Body* createBody();
    static std::string decode(const std::string& str);
};

#endif // !URLENCODED_PARSER_HPP
