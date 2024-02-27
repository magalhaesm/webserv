#ifndef ABODY_PARSER_HPP
#define ABODY_PARSER_HPP

#include "Body.hpp"

class Message;

class ABodyParser
{
public:
    ABodyParser(std::string& raw, Message& msg, size_t maxSize);
    virtual ~ABodyParser();
    virtual bool needsMoreContent();
    virtual Body* createBody() = 0;

protected:
    std::string& _raw;
    Message& _msg;
    BodyContent _content;
    size_t _size;
    size_t _maxSize;

private:
    typedef bool (*TransferMethod)(std::string&, size_t, size_t);
    TransferMethod _stopReading;

    void setStopReadingMethod(Message& msg);
};

#endif // !ABODY_PARSER_HPP
