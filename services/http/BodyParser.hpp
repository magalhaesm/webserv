#ifndef BODY_PARSER_HPP
#define BODY_PARSER_HPP

#include <string>
#include <fstream>

#include "http/Message.hpp"

class Body;

class ABodyParser
{
public:
    ABodyParser(std::string& raw, Message& msg);
    virtual ~ABodyParser();
    virtual bool needsMoreContent() = 0;
    virtual Body* createBody() = 0;

protected:
    std::string& m_raw;
    Message& m_msg;
    BodyContent m_content;
    size_t m_bodySize;
};

class URLEncodedParser : public ABodyParser
{
public:
    URLEncodedParser(std::string& raw, Message& msg);
    bool needsMoreContent();
    Body* createBody();
    static std::string decode(const std::string& str);
};

class FormDataParser : public ABodyParser
{
public:
    FormDataParser(std::string& raw, Message& msg);
    bool needsMoreContent();
    Body* createBody();

private:
    std::string m_boundaryStart;
    std::string m_boundaryEnd;
    static const size_t BOUNDARY_LENGTH = 9;
    std::ofstream file;
};

class ChunkedParser : public ABodyParser
{
public:
    ChunkedParser(std::string& raw, Message& msg);
    bool needsMoreContent();
    Body* createBody();
};

#endif // !BODY_PARSER_HPP
