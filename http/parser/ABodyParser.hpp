#ifndef ABODY_PARSER_HPP
#define ABODY_PARSER_HPP

#include "Body.hpp"
#include "definitions.hpp"

class Message;

class ABodyParser
{
public:
    ABodyParser(std::string& raw, Message& msg);
    virtual ~ABodyParser();
    virtual bool needsMoreContent();
    virtual Body* createBody() = 0;

protected:
    std::string& m_raw;
    Message& m_msg;
    BodyContent m_content;
    size_t m_bodySize;

private:
    typedef bool (*TransferMethod)(std::string& raw, size_t bodySize);
    TransferMethod m_stopReading;

    void setStopReadingMethod(Message& msg);
};

#endif // !ABODY_PARSER_HPP
