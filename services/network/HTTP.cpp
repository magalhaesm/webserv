#include <algorithm>

#include "HTTP.hpp"

std::string toLower(const std::string& input)
{
    std::string s = input;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

BadRequestException::BadRequestException(const std::string& message)
    : std::runtime_error(message)
{
}
