#include <sstream>
#include <unistd.h>

#include "HTTPParser.hpp"
#include "HTTPConstants.hpp"
#include "strings.hpp"

const bool AGAIN = false;
const bool DONE = true;
const std::string FINAL_CHUNK = "0" + DELIMITER;

bool parseHeader(std::string& raw, Message& msg);
void readRequestLine(std::istringstream& stream, Message& msg);
void readHeaders(std::istringstream& stream, Message& msg);
int parseHex(const std::string& strNum);

void setMethod(const std::string& method, Message& msg);
void removeCR(std::string& s);
void setBodySize(Message& msg);

bool HTTPParser::parseRequest(std::string& raw, Message& msg, size_t maxSize)
{
    switch (msg.state)
    {
    case HEADERS:
    {
        if (parseHeader(raw, msg) == false)
        {
            return AGAIN;
        }

        if (msg.state == FINISH)
        {
            return DONE;
        }
        msg.state = BODY;
        return parseRequest(raw, msg, maxSize);
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
            setBodySize(msg);
            msg.state = TRANSFER_CONTROL;
            break;
        }
        return parseRequest(raw, msg, maxSize);
    }
    case TRANSFER_CONTROL:
    {
        if (msg.headers.count("transfer-encoding"))
        {
            std::string encoding = ft::toLower(msg.headers.at("transfer-encoding"));
            if (encoding.find("chunked") != std::string::npos)
            {
                msg.state = CHUNKED;
            }
        }
        else
        {
            msg.state = CONTENT_LENGTH;
        }
        msg.makeBody();
        return parseRequest(raw, msg, maxSize);
    }
    case CONTENT_LENGTH:
    {
        msg.written += write(msg.body, raw.data(), raw.size());
        raw.clear();

        if (msg.written < msg.cLength)
        {
            return AGAIN;
        }
        msg.state = FINISH;
        return DONE;
    }
    case CHUNKED:
    {
        if (raw.find(FINAL_CHUNK) == std::string::npos)
        {
            return AGAIN;
        }
        size_t delim = raw.find(CRLF);
        size_t contentBegin = delim + CRLF.length();
        int contentSize = parseHex(raw) + CRLF.length();
        std::string content = raw.substr(contentBegin, contentSize);
        msg.written += write(msg.body, content.data(), content.length());
        raw.erase();
        return DONE;
    }
    case FINISH:
        return DONE;
    }
    return AGAIN;
}

int parseHex(const std::string& strNum)
{
    int decimal;
    std::istringstream(strNum) >> std::hex >> decimal;
    return decimal;
}

bool parseHeader(std::string& raw, Message& msg)
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
    return DONE;
}

void setBodySize(Message& msg)
{
    if (msg.headers.count("content-length"))
    {
        int clength = std::atoi(msg.headers.at("content-length").c_str());
        msg.cLength = clength;
    }
}

inline void readRequestLine(std::istringstream& stream, Message& msg)
{
    std::string method;

    stream >> method >> msg.path >> msg.version;
    if (method.empty() || msg.path.empty() || msg.version.empty())
    {
        msg.error = BAD_REQUEST;
        msg.state = FINISH;
        return;
    }

    setMethod(method, msg);
    size_t delim = msg.path.find("?");
    if (delim != std::string::npos)
    {
        msg.query = msg.path.substr(delim + 1);
        msg.path.resize(delim);
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
            if (value.empty())
            {
                msg.error = BAD_REQUEST;
                msg.state = FINISH;
                break;
            }
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
