#include <sstream>
#include <algorithm>

#include "strings.hpp"

ft::Strings ft::split(const std::string& input, const std::string& delimiter)
{
    Strings tokens;
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
    Strings tokens = split(input, delimiter);
    return tokens[idx];
}

std::string ft::toLower(const std::string& input)
{
    std::string s = input;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

bool ft::startsWith(const std::string& input, const std::string& prefix)
{
    if (input.length() < prefix.length())
    {
        return false;
    }

    return input.substr(0, prefix.length()) == prefix;
}

inline std::string& ltrim(std::string& s)
{
    s.erase(
        s.begin(),
        std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

inline std::string& rtrim(std::string& s)
{
    s.erase(
        std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(),
        s.end());
    return s;
}

std::string& ft::trim(std::string& s)
{
    return ltrim(rtrim(s));
}

std::string ft::itoa(int n)
{
    std::stringstream s;
    s << n;
    return s.str();
}

ft::Strings ft::strip(const Strings& content)
{
    Strings result;
    for (Strings::const_iterator it = content.begin(); it != content.end(); ++it)
    {
        if (!it->empty())
        {
            result.push_back(*it);
        }
    }
    return result;
}

void ft::replace(std::string& text, const std::string& search, const std::string& replace)
{
    size_t position = 0;
    const size_t searchLength = search.length();
    const size_t replaceLength = replace.length();

    position = text.find(search, position);
    while (position != std::string::npos)
    {
        text.erase(position, searchLength);
        text.insert(position, replace);
        position += replaceLength;
        position = text.find(search, position);
    }
}
