#include "HTTP.hpp"

namespace http
{
    enum httpStatus
    {
        OK = 200

    };
};

std::string toLower(const std::string& input)
{
    std::string result(input);
    for (size_t i = 0; i < input.length(); ++i)
    {
        result[i] = std::tolower(input[i]);
    }
    return result;
}

BadRequestException::BadRequestException(const std::string& message)
    : std::runtime_error(message)
{
}
