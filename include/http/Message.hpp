#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <cstdlib>

#include "HTTPConstants.hpp"

struct Body
{
    int fd;
    std::string filename;
};

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
    bool chunked;
    size_t cLength;
    size_t written;
    int error;

    Message();
    ~Message();
    void clear();
    void makeBody();
};

void clear(Message& msg);

#endif // !MESSAGE_HPP
