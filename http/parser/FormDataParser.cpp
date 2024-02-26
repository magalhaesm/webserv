#include "strings.hpp"
#include "Message.hpp"
#include "definitions.hpp"
#include "FormDataParser.hpp"

const size_t FormDataParser::BOUNDARY_LENGTH = 9;

static void removeOuterQuotes(std::string& str);

FormDataParser::FormDataParser(std::string& raw, Message& msg)
    : ABodyParser(raw, msg)
{
    this->setBoundary();
}

Body* FormDataParser::createBody()
{
    parseRawBody();
    return new Body(FormData, m_content);
}

inline void FormDataParser::setBoundary()
{
    Headers::const_iterator it = m_msg.headers.find("content-type");
    if (it != m_msg.headers.end())
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
            m_boundaryStart = "--" + boundary;
            m_boundaryEnd = m_boundaryStart + "--";
        }
    }
}

inline void FormDataParser::parseRawBody()
{
    size_t end = m_raw.find(DELIMITER);
    if (end != std::string::npos)
    {
        std::string metaData = m_raw.substr(0, end);
        ft::Strings header = ft::split(metaData, CRLF);
        ft::Strings disposition = ft::split(header[1], "; ");

        std::string name = ft::split(disposition[1], "=", 1);
        removeOuterQuotes(name);
        std::string filename = ft::split(disposition[2], "=", 1);
        removeOuterQuotes(filename);

        m_content[name] = filename;

        file.open(filename.c_str());
        m_raw.erase(0, end + DELIMITER.length());
    }
    end = m_raw.find(CRLF + m_boundaryEnd);
    if (end != std::string::npos)
    {
        file << m_raw.substr(0, end);
        file.close();
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
