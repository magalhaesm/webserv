#include <sstream>

#include "Message.hpp"
#include "URLEncodedParser.hpp"

bool isURLHexEncoded(const std::string& str, size_t i);

URLEncodedParser::URLEncodedParser(std::string& raw, Message& msg, size_t maxSize)
    : ABodyParser(raw, msg, maxSize)
{
}

Body* URLEncodedParser::createBody()
{
    _content["content"] = decode(_raw);
    return new Body(URLEncoded, _content);
}

std::string URLEncodedParser::decode(const std::string& str)
{
    std::ostringstream decoded;

    for (size_t i = 0; i < str.length(); ++i)
    {
        if (str[i] == '+')
        {
            decoded << ' ';
        }
        else if (isURLHexEncoded(str, i))
        {
            char hex[3] = { str[i + 1], str[i + 2], '\0' };
            int value;
            std::istringstream(hex) >> std::hex >> value;
            decoded << static_cast<char>(value);
            i += 2;
        }
        else
        {
            decoded << str[i];
        }
    }
    return decoded.str();
}

inline bool isURLHexEncoded(const std::string& str, size_t i)
{
    return str[i] == '%' && i + 2 < str.length()
           && std::isxdigit(static_cast<unsigned char>(str[i + 1]))
           && std::isxdigit(static_cast<unsigned char>(str[i + 2]));
}
