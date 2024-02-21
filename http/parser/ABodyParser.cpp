#include <cstdlib>
#include <iostream>
#include <sstream>

#include "Message.hpp"
#include "ABodyParser.hpp"

const std::string FINAL_CHUNK = "0" + DELIMITER;

void unchunkBody(std::string& raw);

ABodyParser::ABodyParser(std::string& raw, Message& msg)
    : m_raw(raw)
    , m_msg(msg)
    , m_bodySize(0)
{
    Headers::const_iterator it = msg.headers.find("transfer-encoding");
    if (it != msg.headers.end() && it->second.find("chunked") != std::string::npos)
    {
        m_contentCheckMethod = &ABodyParser::findFinalChunk;
        return;
    }

    it = m_msg.headers.find("content-length");
    if (it != m_msg.headers.end())
    {
        m_bodySize = std::atoi(it->second.c_str());
        m_contentCheckMethod = &ABodyParser::readContentLength;
    }
}

ABodyParser::~ABodyParser()
{
}

bool ABodyParser::needsMoreContent()
{
    return (this->*m_contentCheckMethod)();
}

bool ABodyParser::findFinalChunk()
{
    size_t lastChunk = m_raw.rfind(FINAL_CHUNK);
    if (lastChunk == std::string::npos)
    {
        return true;
    }
    unchunkBody(m_raw);
    return false;
}

bool ABodyParser::readContentLength()
{
    return m_raw.size() < m_bodySize;
}

void unchunkBody(std::string& raw)
{
    size_t offset = 0;
    std::string content;

    int decimalSize;
    offset = raw.find(CRLF);
    std::istringstream hexChunkSize(raw.substr(0, offset));
    hexChunkSize >> std::hex >> decimalSize;
    content.append(raw.substr(offset + CRLF.length(), decimalSize));
    raw = content;
}
