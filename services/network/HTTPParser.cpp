#include <cstdlib>
#include <sstream>
#include <iostream>

#include "HTTP.hpp"
#include "HTTPParser.hpp"

void parseRequestLine(std::istringstream& stream, http::Message& msg);
void parseHeaders(std::istringstream& stream, http::Message& msg);

void setMethod(const std::string& method, http::Message& msg);
void removeCR(std::string& s);

const bool INCOMPLETE = false;
const bool COMPLETE = true;

// Return false whenever there isn't enough data to be parsed.
bool HTTPParser::parseRequest(const std::string& raw, http::Message& msg)
{
    switch (msg.state)
    {
    case http::HEADERS:
    {
        size_t end = raw.rfind(HEADER_END);
        if (end == std::string::npos)
        {
            return INCOMPLETE;
        }

        std::istringstream stream(raw.substr(msg.cursor, end));
        parseRequestLine(stream, msg);
        parseHeaders(stream, msg);

        msg.cursor = end + 4;
        msg.state = http::BODY;
        return parseRequest(raw, msg);
    }
    case http::BODY:
    {
        switch (msg.method)
        {
        case http::GET:
        case http::DELETE:
        case http::UNKNOWN:
            msg.state = http::FINISHED;
            break;
        case http::POST:
            msg.state = http::CONTENT_ENCODING;
            break;
        }
        return parseRequest(raw, msg);
    }
    case http::CONTENT_ENCODING:
    {
        http::Headers::iterator it = msg.headers.find("transfer-encoding");
        if (it != msg.headers.end())
        {
            if (it->second.find("chunked") != std::string::npos)
            {
                msg.state = http::CHUNKED;
                return parseRequest(raw, msg);
            }
        }
        msg.state = http::CONTENT_LENGTH;
        return parseRequest(raw, msg);
    }
    case http::CONTENT_LENGTH:
    {
        http::Headers::iterator it = msg.headers.find("content-length");
        size_t remainingData = raw.size() - msg.cursor;
        size_t expectedData = std::atoi(it->second.c_str());
        if (remainingData < expectedData)
        {
            return INCOMPLETE;
        }
        msg.body = raw.substr(msg.cursor);
        std::cout << "BODY:\n" << msg.body << std::endl;
        msg.state = http::FINISHED;
        return COMPLETE;
    }
    case http::CHUNKED:
    {
        msg.state = http::FINISHED;
        std::cout << "Chunked\n";
        return COMPLETE;
    }
    case http::FINISHED:
        return COMPLETE;
    }
    return INCOMPLETE;
}

inline void parseRequestLine(std::istringstream& stream, http::Message& msg)
{
    std::string method;

    stream >> method >> msg.path >> msg.version;
    setMethod(method, msg);
}

inline void parseHeaders(std::istringstream& stream, http::Message& msg)
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

inline void setMethod(const std::string& method, http::Message& msg)
{
    if (method == "GET")
    {
        msg.method = http::GET;
    }
    else if (method == "POST")
    {
        msg.method = http::POST;
    }
    else if (method == "DELETE")
    {
        msg.method = http::DELETE;
    }
    else
    {
        msg.method = http::UNKNOWN;
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
