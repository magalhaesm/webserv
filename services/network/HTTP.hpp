#ifndef HTTP_HPP
#define HTTP_HPP

#include <map>
#include <string>
#include <stdexcept>

typedef std::map<std::string, std::string> Headers;

const std::string CRLF("\r\n");

std::string toLower(const std::string& input);

class BadRequestException : public std::runtime_error
{
public:
    BadRequestException(const std::string& message);
};

#endif // !HTTP_HPP
