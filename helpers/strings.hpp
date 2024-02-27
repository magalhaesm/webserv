#ifndef STRINGS_HPP
#define STRINGS_HPP

#include <string>
#include <vector>

namespace ft
{
    typedef std::vector<std::string> Strings;

    Strings split(const std::string& input, const std::string& delimiter);
    std::string split(const std::string& input, const std::string& delimiter, int idx);
    std::string toLower(const std::string& input);
    std::string& trim(std::string& s);
    bool startsWith(const std::string& input, const std::string& prefix);
    std::string itoa(int n);
    Strings strip(const Strings& content);
    void replace(std::string& text, const std::string& search, const std::string& replace);

}
#endif // !STRING_HPP
