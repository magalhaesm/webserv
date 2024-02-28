#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include "strings.hpp"

namespace ft
{
    bool isDir(const std::string& path);
    Strings listDir(const std::string& dirname);
}

#endif // !FILESYSTEM_HPP
