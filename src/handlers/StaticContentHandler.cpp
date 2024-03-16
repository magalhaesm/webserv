#include <fstream>
#include <sstream>

#include "Request.hpp"
#include "Response.hpp"
#include "filesystem.hpp"
#include "HTTPConstants.hpp"
#include "StaticContentHandler.hpp"

std::string decode(const std::string& str);
inline bool isURLHexEncoded(const std::string& str, size_t i);

StaticContentHandler::StaticContentHandler()
{
}

void StaticContentHandler::handle(Request& req, Response& res, const ConfigSpec& cfg)
{
    switch (req.method())
    {
    case GET:
        handleGet(req, res, cfg);
        break;
    case POST:
        handlePost(req, res, cfg);
        break;
    case DELETE:
        handleDelete(req, res, cfg);
        break;
    default:
        sendStatusPage(METHOD_NOT_ALLOWED, res, cfg);
    }

    if (_next)
    {
        _next->handle(req, res, cfg);
    }
}

void StaticContentHandler::handleGet(Request& req, Response& res, const ConfigSpec& cfg)
{
    std::ifstream resource(decode(req.realPath()).c_str());
    if (resource.is_open())
    {
        res.setStatus(OK);
        res.setBody(resource);
        return;
    }
    sendStatusPage(NOT_FOUND, res, cfg);
}

void StaticContentHandler::handlePost(Request&, Response& res, const ConfigSpec& cfg)
{
    sendStatusPage(NOT_IMPLEMENTED, res, cfg);
}

void StaticContentHandler::handleDelete(Request& req, Response& res, const ConfigSpec& cfg)
{
    std::string filepath = decode(req.realPath());
    if (ft::isDir(filepath))
    {
        sendStatusPage(FORBIDDEN, res, cfg);
        return;
    }
    if (remove(filepath.c_str()) != 0)
    {
        sendStatusPage(INTERNAL_SERVER_ERROR, res, cfg);
        return;
    }
    sendStatusPage(OK, res, cfg);
}

std::string decode(const std::string& str)
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
