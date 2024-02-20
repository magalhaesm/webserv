#include <algorithm>

#include "strings.hpp"

ft::StringArray ft::split(const std::string& input, const std::string& delimiter)
{
    StringArray tokens;
    size_t start = 0;
    size_t end = input.find(delimiter);

    while (end != std::string::npos)
    {
        tokens.push_back(input.substr(start, end - start));
        start = end + delimiter.length();
        end = input.find(delimiter, start);
    }

    tokens.push_back(input.substr(start));
    return tokens;
}

std::string ft::split(const std::string& input, const std::string& delimiter, int idx)
{
    StringArray tokens = split(input, delimiter);
    return tokens[idx];
}

std::string ft::toLower(const std::string& input)
{
    std::string s = input;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}
