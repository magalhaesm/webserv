#include <cstdlib>
#include <sstream>
#include <iostream>

#include "helpers.hpp"
#include "BodyParser.hpp"
#include "HTTPParser.hpp"

void readRequestLine(std::istringstream& stream, Message& msg);
void readHeaders(std::istringstream& stream, Message& msg);

void setMethod(const std::string& method, Message& msg);
void removeCR(std::string& s);

const bool AGAIN = false;
const bool DONE = true;

// NOTE:
// urlEncoded = value
// chunked = value
// name = value/@filename

// Return false whenever there isn't enough data to be parsed.
// bool HTTPParser::parseRequest(const std::string& raw, Message& msg, const ConfigSpec& cfg);
bool HTTPParser::parseRequest(const std::string& raw, Message& msg)
{
    switch (msg.state)
    {
    case HEADERS:
    {
        size_t end = raw.rfind(DELIMITER);
        if (end == std::string::npos)
        {
            return AGAIN;
        }

        std::istringstream stream(raw.substr(msg.offset, end));
        readRequestLine(stream, msg);
        readHeaders(stream, msg);

        msg.offset = end + DELIMITER.length();
        msg.state = BODY;
        return parseRequest(raw, msg);
    }
    case BODY:
    {
        switch (msg.method)
        {
        case GET:
        case DELETE:
        case UNKNOWN:
            msg.state = FINISH;
            break;
        case POST:
            msg.state = CONTENT_TYPE;
            break;
        }
        return parseRequest(raw, msg);
    }
    case CONTENT_TYPE:
    {
        Headers::iterator it = msg.headers.find("transfer-encoding");
        if (it != msg.headers.end())
        {
            if (it->second.find("chunked") != std::string::npos)
            {
                msg.state = CHUNKED_DATA;
                return parseRequest(raw, msg);
            }
        }

        it = msg.headers.find("content-type");
        if (it != msg.headers.end())
        {
            if (it->second.find("x-www-form-urlencoded") != std::string::npos)
            {
                msg.parser = new URLEncodedParser(raw, msg);
                msg.state = URL_ENCODED;
            }
            else if (it->second.find("multipart/form-data") != std::string::npos)
            {
                // TODO: mover para o parser do body
                size_t boundaryPos = it->second.find("boundary") + 9;
                if (boundaryPos != std::string::npos)
                {
                    msg.state = FORM_DATA;
                }
            }
        }
        return parseRequest(raw, msg);
    }
    case URL_ENCODED:
    {
        if (msg.parser->needsMoreContent())
        {
            return AGAIN;
        }
        msg.body = msg.parser->createBody();
        msg.state = FINISH;
        return DONE;
    }
    // Criar arquivo temporário, escrever nele
    // NOTE: colocar em body apenas o nome do arquivo já baixado em /tmp
    case FORM_DATA:
    {
        msg.state = FINISH;
        return DONE;
    }
    case CHUNKED_DATA:
    {
        msg.state = FINISH;
        std::cout << "Chunked\n";
        return DONE;
    }
    case FINISH:
        return DONE;
    }
    return AGAIN;
}

inline void readRequestLine(std::istringstream& stream, Message& msg)
{
    std::string method;

    stream >> method >> msg.path >> msg.version;
    setMethod(method, msg);
}

inline void readHeaders(std::istringstream& stream, Message& msg)
{
    std::string line;

    while (std::getline(stream, line) && !line.empty())
    {
        removeCR(line);
        size_t colonPos = line.find(": ");
        if (colonPos != std::string::npos)
        {
            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 2);
            msg.headers[toLower(key)] = value;
        }
    }
}

inline void setMethod(const std::string& method, Message& msg)
{
    if (method == "GET")
    {
        msg.method = GET;
    }
    else if (method == "POST")
    {
        msg.method = POST;
    }
    else if (method == "DELETE")
    {
        msg.method = DELETE;
    }
    else
    {
        msg.method = UNKNOWN;
    }
}

inline void removeCR(std::string& s)
{
    size_t len = s.length() - 1;
    if (s[len] == '\r')
    {
        s.resize(len);
    }
}
