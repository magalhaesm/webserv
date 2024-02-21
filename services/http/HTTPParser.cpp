#include <cstdlib>
#include <sstream>

#include "definitions.hpp"
#include "strings.hpp"
#include "Message.hpp"
#include "BodyParser.hpp"
#include "HTTPParser.hpp"

const bool AGAIN = false;
const bool DONE = true;

void readRequestLine(std::istringstream& stream, Message& msg);
void readHeaders(std::istringstream& stream, Message& msg);

void setMethod(const std::string& method, Message& msg);
void removeCR(std::string& s);
bool has(const std::string& term, const Headers::const_iterator& it);

// Return false whenever there isn't enough data to be parsed.
// bool HTTPParser::parseRequest(const std::string& raw, Message& msg, const ConfigSpec& cfg);
bool HTTPParser::parseRequest(std::string& raw, Message& msg)
{
    switch (msg.state)
    {
    case HEADERS:
    {
        size_t end = raw.find(DELIMITER);
        if (end == std::string::npos)
        {
            return AGAIN;
        }

        std::istringstream stream(raw.substr(0, end));
        readRequestLine(stream, msg);
        readHeaders(stream, msg);

        raw.erase(0, end + DELIMITER.length());
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
            msg.state = BODY_TYPE;
            break;
        }
        return parseRequest(raw, msg);
    }
    case BODY_TYPE:
    {
        Headers::const_iterator it = msg.headers.find("transfer-encoding");
        if (it != msg.headers.end())
        {
            if (has("chunked", it))
            {
                msg.parser = new ChunkedParser(raw, msg);
            }
        }
        else
        {
            it = msg.headers.find("content-type");
            if (it != msg.headers.end())
            {
                if (has("x-www-form-urlencoded", it))
                {
                    msg.parser = new URLEncodedParser(raw, msg);
                }
                else if (has("multipart/form-data", it))
                {
                    msg.parser = new FormDataParser(raw, msg);
                }
            }
        }
        msg.state = BODY_CONTENT;
        return parseRequest(raw, msg);
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
