#include <algorithm>

#include "HTTP.hpp"

http::Message::Message()
{
    this->clear();
}

void http::Message::clear()
{
    // state = INITIAL;
    // cursor = 0;
    method = UNKNOWN;
    version.clear();
    path.clear();
    query.clear();
    headers.clear();
    body.clear();
    state = HEADERS;
    cursor = 0;
}

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
