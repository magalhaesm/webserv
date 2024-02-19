#include <algorithm>

#include "helpers.hpp"

std::string toLower(const std::string& input)
{
    std::string s = input;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}
