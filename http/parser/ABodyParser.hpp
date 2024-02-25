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
    typedef bool (ABodyParser::*CheckMethod)();
    CheckMethod m_contentCheckMethod;

    bool findFinalChunk();
    bool readContentLength();
};

#endif // !ABODY_PARSER_HPP
