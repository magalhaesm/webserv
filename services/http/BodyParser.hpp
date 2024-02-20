#ifndef BODY_PARSER_HPP
#define BODY_PARSER_HPP

#include <fstream>
#include <string>
#include "http/Message.hpp"

class Body;

// TODO: modificar o construtor de Body para receber BodyContent
// e remover o método set
class ABodyParser
{
public:
    ABodyParser(const std::string& raw, Message& msg);
    virtual ~ABodyParser();
    virtual bool needsMoreContent();
    virtual Body* createBody() = 0;

protected:
    const std::string& m_raw;
    Message& m_msg;
    BodyContent m_content;
    size_t m_bodySize;
};

class URLEncodedParser : public ABodyParser
{
public:
    URLEncodedParser(const std::string& raw, Message& msg);
    Body* createBody();
    static std::string decode(const std::string& str);
};

class FormDataParser : public ABodyParser
{
public:
    FormDataParser(const std::string& raw, Message& msg);
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
    ChunkedParser(const std::string& raw, Message& msg);
    Body* createBody();
};

#endif // !BODY_PARSER_HPP
