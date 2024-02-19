#ifndef BODY_PARSER_HPP
#define BODY_PARSER_HPP

#include <string>

class Body;
struct Message;

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
    Body* m_body;
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
    Body* createBody();

private:
    enum ParsingPhase
    {
        Meta,   // CRLF
        Content // Data
    };
    ParsingPhase m_state;
};

class ChunkedParser : public ABodyParser
{
public:
    ChunkedParser(const std::string& raw, Message& msg);
    Body* createBody();
};

#endif // !BODY_PARSER_HPP
