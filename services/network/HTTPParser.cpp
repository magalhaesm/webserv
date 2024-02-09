#include "HTTPParser.hpp"

HTTPParser::HTTPParser()
    : m_request(m_msg)
    , m_bodySizeInBytes(32)
{
    m_bodySizeInBytes <<= 20;
}

HTTPParser::HTTPParser(unsigned int bodySize)
    : m_request(m_msg)
{
    m_bodySizeInBytes = bodySize << 20;
}

HTTPParser::~HTTPParser()
{
}

const HTTPRequest& HTTPParser::newHTTPRequest()
{
    return m_request = HTTPRequest(m_msg);
}

bool HTTPParser::isRequestComplete(const std::string& buffer)
{
    size_t pos = buffer.rfind("\r\n\r\n");
    if (pos != std::string::npos)
    {
        std::istringstream stream(buffer);
        parseRequestLine(stream);
        parseHeaders(stream);
        return true;
    }
    return false;
}

void HTTPParser::parseRequestLine(std::istringstream& stream)
{
    stream >> m_msg.method >> m_msg.path >> m_msg.version;
    // stream >> m_msg.method >> m_msg.path >> m_msg.version;
    // if (method == "GET")
    // {
    //     m_msg.m_method = http::GET;
    // }
    // else if (method == "POST")
    // {
    //     m_msg.m_method = http::POST;
    // }
    // else if (method == "DELETE")
    // {
    //     m_msg.m_method = http::DELETE;
    // }
}

void HTTPParser::parseHeaders(std::istringstream& stream)
{
    std::string line;

    while (std::getline(stream, line) && !line.empty())
    {
        if (line[line.length() - 1] == '\r')
        {
            line.resize(line.length() - 1);
        }

        size_t colonPos = line.find(": ");
        if (colonPos != std::string::npos)
        {
            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 2);
            m_msg.headers[toLower(key)] = value;
        }
    }
}
