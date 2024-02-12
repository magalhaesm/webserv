#include "HTTP.hpp"
#include "HTTPParser.hpp"
#include <iostream>

HTTPParser::HTTPParser()
    : m_request(m_msg)
    , m_bodySizeInBytes(32)
{
}

HTTPParser::HTTPParser(unsigned int bodySize)
    : m_request(m_msg)
{
    m_bodySizeInBytes = bodySize << 20;
}

HTTPParser::~HTTPParser()
{
}

// TODO: em caso de erro de parsing, retornar um objeto HTTPError
const HTTPRequest& HTTPParser::newHTTPRequest()
{
    return m_request = HTTPRequest(m_msg);
}

// TODO: implementar o parse do body usando máquina de estados
// armazenar os estados do parsing no objeto Message
// bool HTTPParser::parseRequest(Message msg, const std::string& data)
bool HTTPParser::parseRequest(const std::string& data)
{
    // NOTE:
    // pos é a posição do fim dos cabeçalhos e início do corpo
    size_t pos = data.rfind("\r\n\r\n");
    if (pos != std::string::npos)
    {
        std::istringstream stream(data);
        parseRequestLine(stream);
        parseHeaders(stream);
        return true;
    }
    return false;
}

inline void HTTPParser::parseRequestLine(std::istringstream& stream)
{
    std::string method;
    stream >> method >> m_msg.path >> m_msg.version;

    if (method == "GET")
    {
        m_msg.method = http::GET;
    }
    else if (method == "POST")
    {
        m_msg.method = http::POST;
    }
    else if (method == "DELETE")
    {
        m_msg.method = http::DELETE;
    }
}

inline void HTTPParser::parseHeaders(std::istringstream& stream)
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

inline void HTTPParser::parseBody(const std::string& body)
{
    std::cout << body << std::endl;
    // State state = GET;
    //
    // while (state != FINISHED)
    // {
    //     switch (state)
    //     {
    //     default:
    //
    //         break;
    //     }
    // }
}
