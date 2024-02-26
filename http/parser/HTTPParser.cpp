#include <cstdlib>
#include <sstream>

#include "HTTPParser.hpp"
#include "FormDataParser.hpp"
#include "URLEncodedParser.hpp"
#include "HTTPConstants.hpp"
#include "strings.hpp"

const bool AGAIN = false;
const bool DONE = true;

void readRequestLine(std::istringstream& stream, Message& msg);
void readHeaders(std::istringstream& stream, Message& msg);
void setBodySize(Message& msg);

void setMethod(const std::string& method, Message& msg);
void removeCR(std::string& s);
bool has(const std::string& term, const Headers::const_iterator& it);

bool HTTPParser::parseHeader(std::string& raw, Message& msg)
{
    size_t end = raw.find(DELIMITER);
    if (end == std::string::npos)
    {
        return AGAIN;
    }

    std::istringstream stream(raw.substr(0, end));
    readRequestLine(stream, msg);
    readHeaders(stream, msg);
    setBodySize(msg);

    raw.erase(0, end + DELIMITER.length());
    return DONE;
}

void setBodySize(Message& msg)
{
    if (msg.headers.count("content-length"))
    {
        int bodySize = std::atoi(msg.headers.at("content-length").c_str());
        msg.bodySize = bodySize;
    }
}

bool HTTPParser::parseBody(std::string& raw, Message& msg, int maxSize)
{
    switch (msg.state)
    {
    case HEADERS:
    case BODY_TYPE:
    {
        Headers::const_iterator it = msg.headers.find("content-type");
        if (it != msg.headers.end())
        {
            if (has("x-www-form-urlencoded", it))
            {
                msg.parser = new URLEncodedParser(raw, msg, maxSize);
            }
            else if (has("multipart/form-data", it))
            {
                msg.parser = new FormDataParser(raw, msg, maxSize);
            }
        }
        msg.state = BODY_CONTENT;
        return parseBody(raw, msg, maxSize);
    }
    case BODY_CONTENT:
    {
        if (msg.parser->needsMoreContent())
        {
            return AGAIN;
        }
        msg.body = msg.parser->createBody();
        msg.state = FINISH;
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

    size_t question = msg.path.find("?");
    if (question != std::string::npos)
    {
        msg.query = URLEncodedParser::decode(msg.path.substr(question + 1));
        msg.path = msg.path.substr(0, question);
    }
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
            msg.headers[ft::toLower(key)] = value;
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

inline bool has(const std::string& term, const Headers::const_iterator& it)
{
    return it->second.find(term) != std::string::npos;
}
