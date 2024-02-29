#include <cstdlib>
#include <sstream>

#include "Message.hpp"
#include "ABodyParser.hpp"

const std::string FINAL_CHUNK = "0" + DELIMITER;

void unchunkBody(std::string& raw);
bool findFinalChunk(std::string& raw, size_t size, size_t maxSize);
bool readContentLength(std::string& raw, size_t size, size_t maxSize);

ABodyParser::ABodyParser(std::string& raw, Message& msg, size_t maxSize)
    : _raw(raw)
    , _msg(msg)
    , _size(msg.bodySize)
    , _maxSize(maxSize)
{
    setStopReadingMethod(msg);
}

ABodyParser::~ABodyParser()
{
}

bool ABodyParser::needsMoreContent()
{
    if (_maxSize > 0 && _raw.size() > _maxSize)
    {
        _msg.error = 413;
    }

    return _stopReading(_raw, _size, _maxSize);
}

inline void ABodyParser::setStopReadingMethod(Message& msg)
{
    Headers::const_iterator it = msg.headers.find("transfer-encoding");
    if (it != msg.headers.end() && it->second.find("chunked") != std::string::npos)
    {
        _stopReading = &findFinalChunk;
        return;
    }
    _stopReading = &readContentLength;
}

bool findFinalChunk(std::string& raw, size_t, size_t)
{
    size_t lastChunk = raw.rfind(FINAL_CHUNK);
    if (lastChunk == std::string::npos)
    {
        return true;
    }
    unchunkBody(raw);
    return false;
}

// PERF: para descartar o excesso, preciso controlar quanto foi lido
bool readContentLength(std::string& raw, size_t size, size_t)
{
    return raw.size() < size;
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
