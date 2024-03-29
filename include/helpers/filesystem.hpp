#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include "strings.hpp"

namespace ft
{
    bool isDir(const std::string& path);
    Strings scanDir(const std::string& dirname);
}

#endif // !FILESYSTEM_HPP
