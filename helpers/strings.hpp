#ifndef STRINGS_HPP
#define STRINGS_HPP

#include <string>
#include <vector>

namespace ft
{
    typedef std::vector<std::string> StringArray;

    StringArray split(const std::string& input, const std::string& delimiter);
    std::string split(const std::string& input, const std::string& delimiter, int idx);

    std::string toLower(const std::string& input);
}
#endif // !STRING_HPP
