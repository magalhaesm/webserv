#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>

#include "Body.hpp"

class ABodyParser;

struct Message
{
    Method method;
    std::string version;
    std::string path;
    std::string query;
    Headers headers;
    Body* body;
    int bodySize;
    int error;
    ABodyParser* parser;
    ParsingState state;

    Message();
};

void clear(Message& msg);

#endif // !MESSAGE_HPP
