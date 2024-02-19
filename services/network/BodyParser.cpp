#include <cstdlib>
#include <sstream>

#include "Message.hpp"
#include "BodyParser.hpp"

static bool isURLHexEncoded(const std::string& str, size_t i);

// ------------------------------------------------------------------------

ABodyParser::ABodyParser(const std::string& raw, Message& msg)
    : m_raw(raw)
    , m_msg(msg)
    , m_body(NULL)
    , m_bodySize(0)
{
    Headers::iterator it = m_msg.headers.find("content-length");
    if (it != m_msg.headers.end())
    {
        m_bodySize = std::atoi(it->second.c_str());
    }
}

bool ABodyParser::needsMoreContent()
{
    size_t contentLength = m_raw.size() - m_msg.offset;
    return contentLength < m_bodySize;
}

ABodyParser::~ABodyParser()
{
    if (m_body != NULL)
    {
        delete m_body;
    }
}

// ------------------------------------------------------------------------

URLEncodedParser::URLEncodedParser(const std::string& raw, Message& msg)
    : ABodyParser(raw, msg)
{
}

Body* URLEncodedParser::createBody()
{
    Body* body = new Body(URLEncoded);
    body->set("content", decode(m_raw.substr(m_msg.offset)));
    return body;
}

std::string URLEncodedParser::decode(const std::string& str)
{
    std::ostringstream decoded;

    for (size_t i = 0; i < str.length(); ++i)
    {
        if (str[i] == '+')
        {
            decoded << ' ';
        }
        else if (isURLHexEncoded(str, i))
        {
            char hex[3] = { str[i + 1], str[i + 2], '\0' };
            int value;
            std::istringstream(hex) >> std::hex >> value;
            decoded << static_cast<char>(value);
            i += 2;
        }
        else
        {
            decoded << str[i];
        }
    }
    return decoded.str();
}

inline bool isURLHexEncoded(const std::string& str, size_t i)
{
    return str[i] == '%' && i + 2 < str.length()
           && std::isxdigit(static_cast<unsigned char>(str[i + 1]))
           && std::isxdigit(static_cast<unsigned char>(str[i + 2]));
}

// ------------------------------------------------------------------------

FormDataParser::FormDataParser(const std::string& raw, Message& msg)
    : ABodyParser(raw, msg)
{
}

Body* FormDataParser::createBody()
{
    Body* body = new Body(FormData);
    body->set("content", m_raw.substr(m_msg.offset));
    return body;
}

// ------------------------------------------------------------------------

ChunkedParser::ChunkedParser(const std::string& raw, Message& msg)
    : ABodyParser(raw, msg)
{
}

Body* ChunkedParser::createBody()
{
    Body* body = new Body(Chunked);
    body->set("content", m_raw.substr(m_msg.offset));
    return body;
}

// ------------------------------------------------------------------------
