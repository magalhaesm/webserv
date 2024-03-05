#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <stdexcept>
#include <strings.h>
#include <sys/stat.h>

#include "strings.hpp"
#include "filesystem.hpp"

bool ft::isDir(const std::string& path)
{
    struct stat statbuf;
    bzero(&statbuf, sizeof(statbuf));
    stat(path.c_str(), &statbuf);
    return (statbuf.st_mode & S_IFMT) == S_IFDIR;
}

ft::Strings ft::listDir(const std::string& dirname)
{
    DIR* dir = opendir(dirname.c_str());
    if (dir == NULL)
    {
        throw std::runtime_error(strerror(errno));
    }

    Strings filenames;
    struct dirent* ent;
    while ((ent = readdir(dir)) != NULL)
    {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
        {
            continue;
        }

        std::string filename = ent->d_name;
        if (ent->d_type == DT_DIR)
        {
            filename += "/";
        }
        filenames.push_back(filename);
    }

    closedir(dir);
    return filenames;
}
