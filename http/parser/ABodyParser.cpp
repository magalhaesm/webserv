#include <cstdlib>
#include <sstream>

#include "Message.hpp"
#include "ABodyParser.hpp"

const std::string FINAL_CHUNK = "0" + DELIMITER;

void unchunkBody(std::string& raw);
bool findFinalChunk(std::string& raw, size_t bodySize);
bool readContentLength(std::string& raw, size_t bodySize);

ABodyParser::ABodyParser(std::string& raw, Message& msg)
    : m_raw(raw)
    , m_msg(msg)
    , m_bodySize(msg.bodySize)
{
    setStopReadingMethod(msg);
}

ABodyParser::~ABodyParser()
{
}

bool ABodyParser::needsMoreContent()
{
    return m_stopReading(m_raw, m_bodySize);
}

inline void ABodyParser::setStopReadingMethod(Message& msg)
{
    Headers::const_iterator it = msg.headers.find("transfer-encoding");
    if (it != msg.headers.end() && it->second.find("chunked") != std::string::npos)
    {
        m_stopReading = &findFinalChunk;
        return;
    }

    m_stopReading = &readContentLength;
}

bool findFinalChunk(std::string& raw, size_t)
{
    size_t lastChunk = raw.rfind(FINAL_CHUNK);
    if (lastChunk == std::string::npos)
    {
        return true;
    }
    unchunkBody(raw);
    return false;
}

bool readContentLength(std::string& raw, size_t bodySize)
{
    return raw.size() < bodySize;
}

// TODO: must be a loop
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
