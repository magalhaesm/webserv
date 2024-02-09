#include "HTTP.hpp"

std::string toLower(const std::string& input)
{
    std::string result(input);
    // std::transform(result.begin(), result.end(), result.begin(), ::tolower);
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

http::Message::Message()
{
}

http::Message::Message(const Message& rhs)
    : method(rhs.method)
    , version(rhs.version)
    , path(rhs.path)
    , query(rhs.query)
    , headers(rhs.headers)
    , body(rhs.body)
{
}

http::Message& http::Message::operator=(const Message& rhs)
{
    if (&rhs != this)
    {
        *this = rhs;
    }

    return *this;
};
http::Message::~Message()
{
}
