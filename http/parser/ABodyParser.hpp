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
    std::string& m_raw;
    Message& m_msg;
    BodyContent m_content;
    size_t m_size;
    size_t m_maxSize;

private:
    typedef bool (*TransferMethod)(std::string&, size_t, size_t);
    TransferMethod m_stopReading;

    void setStopReadingMethod(Message& msg);
};

#endif // !ABODY_PARSER_HPP
