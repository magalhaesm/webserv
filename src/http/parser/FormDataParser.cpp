#include "strings.hpp"
#include "Message.hpp"
#include "HTTPConstants.hpp"
#include "FormDataParser.hpp"

const size_t FormDataParser::BOUNDARY_LENGTH = 9;

static void removeOuterQuotes(std::string& str);

FormDataParser::FormDataParser(std::string& raw, Message& msg, size_t maxSize)
    : ABodyParser(raw, msg, maxSize)
{
    this->setBoundary();
}

Body* FormDataParser::createBody()
{
    parseRawBody();
    return new Body(FormData, _content);
}

inline void FormDataParser::setBoundary()
{
    Headers::const_iterator it = _msg.headers.find("content-type");
    if (it != _msg.headers.end())
    {
        size_t boundaryPos = it->second.find("boundary");
        if (boundaryPos != std::string::npos)
        {
            size_t start = boundaryPos + BOUNDARY_LENGTH;
            size_t end = it->second.find(";", start);
            if (end == std::string::npos)
            {
                end = it->second.find(CRLF, start);
            }
            std::string boundary = it->second.substr(start, end - start);
            _boundaryStart = "--" + boundary;
            _boundaryEnd = _boundaryStart + "--";
        }
    }
}

inline void FormDataParser::parseRawBody()
{
    size_t end = _raw.find(DELIMITER);
    if (end != std::string::npos)
    {
        std::string metaData = _raw.substr(0, end);
        ft::Strings header = ft::split(metaData, CRLF);
        ft::Strings disposition = ft::split(header[1], "; ");

        std::string name = ft::split(disposition[1], "=", 1);
        removeOuterQuotes(name);
        std::string filename = ft::split(disposition[2], "=", 1);
        removeOuterQuotes(filename);

        _content[name] = filename;

        _file.open(filename.c_str());
        _raw.erase(0, end + DELIMITER.length());
    }
    end = _raw.find(CRLF + _boundaryEnd);
    if (end != std::string::npos)
    {
        _file << _raw.substr(0, end);
        _file.close();
    }
}

void removeOuterQuotes(std::string& str)
{
    if (!str.empty() && str[0] == '"' && str[str.length() - 1] == '"')
    {
        str.erase(0, 1);
        str.erase(str.length() - 1);
    }
}