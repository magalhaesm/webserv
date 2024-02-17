#include <cstdlib>
#include <sstream>
#include <iostream>

#include "HTTP.hpp"
#include "HTTPParser.hpp"

void readRequestLine(std::istringstream& stream, http::Message& msg);
void readHeaders(std::istringstream& stream, http::Message& msg);

void setMethod(const std::string& method, http::Message& msg);
void removeCR(std::string& s);
bool needsMoreContent(const std::string& raw, http::Message& msg);

const bool AGAIN = false;
const bool DONE = true;

// Return false whenever there isn't enough data to be parsed.
// bool HTTPParser::parseRequest(const std::string& raw, Message& msg, const ConfigSpec& cfg);
bool HTTPParser::parseRequest(const std::string& raw, http::Message& msg)
{
    switch (msg.state)
    {
    case http::HEADERS:
    {
        size_t end = raw.rfind(PART_END);
        if (end == std::string::npos)
        {
            return AGAIN;
        }

        std::istringstream stream(raw.substr(msg.cursor, end));
        readRequestLine(stream, msg);
        readHeaders(stream, msg);

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
            msg.state = http::FINISH;
            break;
        case http::POST:
            msg.state = http::CONTENT_TYPE;
            break;
        }
        return parseRequest(raw, msg);
    }
    case http::CONTENT_TYPE:
    {
        http::Headers::iterator it = msg.headers.find("transfer-encoding");
        if (it != msg.headers.end())
        {
            if (it->second.find("chunked") != std::string::npos)
            {
                msg.state = http::CHUNKED_DATA;
                return parseRequest(raw, msg);
            }
        }

        it = msg.headers.find("content-type");
        if (it != msg.headers.end())
        {
            if (it->second.find("x-www-form-urlencoded") != std::string::npos)
            {
                msg.state = http::URL_ENCODED;
            }
            else if (it->second.find("multipart/form-data") != std::string::npos)
            {
                msg.state = http::FORM_DATA;
            }
            it = msg.headers.find("content-length");
            msg.bodySize = std::atoi(it->second.c_str());
        }
        return parseRequest(raw, msg);
    }
    case http::URL_ENCODED:
    {
        if (needsMoreContent(raw, msg))
        {
            return AGAIN;
        }
        msg.body = raw.substr(msg.cursor);
        msg.state = http::FINISH;
        return DONE;
    }
    case http::FORM_DATA:
    {
        if (needsMoreContent(raw, msg))
        {
            return AGAIN;
        }
        msg.body = raw.substr(msg.cursor);
        msg.state = http::FINISH;
        return DONE;
    }
    case http::CHUNKED_DATA:
    {
        msg.state = http::FINISH;
        std::cout << "Chunked\n";
        return DONE;
    }
    case http::FINISH:
        return DONE;
    }
    return AGAIN;
}

inline void readRequestLine(std::istringstream& stream, http::Message& msg)
{
    std::string method;

    stream >> method >> msg.path >> msg.version;
    setMethod(method, msg);
}

inline void readHeaders(std::istringstream& stream, http::Message& msg)
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

inline bool needsMoreContent(const std::string& raw, http::Message& msg)
{
    size_t contentLength = raw.size() - msg.cursor;
    return contentLength < msg.bodySize;
}
