#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <cstdlib>

#include "HTTPConstants.hpp"

struct Message
{
    Method method;
    std::string version;
    std::string path;
    std::string query;
    Headers headers;
    int body;
    std::string bodyFilename;
    ParsingState state;
    size_t clength;
    size_t written;
    int error;

    Message();
    ~Message();
    void clear();
    void makeBody();
};

#endif // !MESSAGE_HPP
