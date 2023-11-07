#include "HTTPResponse.hpp"

#include <sstream>

std::string HTTPResponse::HTTPResponse::toString() const
{
    std::ostringstream oss;
    oss << "HTTP/1.1 " << statusCode << " " << contentType << "\r\n";
    oss << "Content-Length: " << body.length() << "\r\n";
    oss << "\r\n";
    oss << body;
    return oss.str();
}
