#include <cstdlib>
#include <sstream>
#include <iostream>

#include "strings.hpp"
#include "definitions.hpp"
#include "Message.hpp"
#include "BodyParser.hpp"

static bool isURLHexEncoded(const std::string& str, size_t i);

// ------------------------------------------------------------------------

ABodyParser::ABodyParser(const std::string& raw, Message& msg)
    : m_raw(raw)
    , m_msg(msg)
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
    Headers::const_iterator it = m_msg.headers.find("content-type");
    if (it != m_msg.headers.end())
    {
        size_t boundaryPos = it->second.find("boundary");
        if (boundaryPos != std::string::npos)
        {
            size_t start = boundaryPos + BOUNDARY_LENGTH;
            size_t end = it->second.find(";", start);
            if (end == std::string::npos)
            {
                end = it->second.find(CRLF, start);
            }
            std::string boundary = it->second.substr(start, end - start);
            m_boundaryStart = "--" + boundary;
            m_boundaryEnd = m_boundaryStart + "--";
        }
    }
}

void removeOuterQuotes(std::string& str)
{
    if (!str.empty() && str[0] == '"' && str[str.length() - 1] == '"')
    {
        str.erase(0, 1);
        str.erase(str.length() - 1);
    }
}

bool FormDataParser::needsMoreContent()
{
    size_t contentLength = m_raw.size() - m_msg.offset;

    if (contentLength < m_bodySize)
    {
        return true;
    }

    size_t end = m_raw.find(DELIMITER, m_msg.offset);
    if (end != std::string::npos)
    {
        std::string metaData = m_raw.substr(m_msg.offset, end);
        ft::StringArray header = ft::split(metaData, CRLF);
        ft::StringArray disposition = ft::split(header[1], "; ");

        std::string name = ft::split(disposition[1], "=", 1);
        removeOuterQuotes(name);
        std::string filename = ft::split(disposition[2], "=", 1);
        removeOuterQuotes(filename);

        m_content[name] = filename;

        file.open(filename.c_str());
        m_msg.offset = end + DELIMITER.length();
    }
    size_t idx = m_raw.find(CRLF + m_boundaryEnd, m_msg.offset);
    if (idx != std::string::npos)
    {
        file << m_raw.substr(m_msg.offset, idx - m_msg.offset);
        file.close();
    }
    return false;
}

Body* FormDataParser::createBody()
{
    Body* body = new Body(FormData);
    body->set("content", m_raw.substr(m_msg.offset));
    // std::cout << m_raw.substr(m_msg.offset) << std::endl;
    // return new Body(FormData, m_content);
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
