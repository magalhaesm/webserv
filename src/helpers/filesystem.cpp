#include <sys/stat.h>

#include "filesystem.hpp"

bool ft::isDir(const std::string& path)
{
    struct stat statbuf;
    stat(path.c_str(), &statbuf);
    if ((statbuf.st_mode & S_IFMT) == S_IFDIR)
    {
        return true;
    }
    return false;
}
