#include "HTTPConstants.hpp"

const std::string& statusText(int code)
{
    static std::map<int, std::string> status;

    status[100] = "Continue";
    status[200] = "OK";
    status[201] = "Created";
    status[301] = "Moved Permanently";
    status[400] = "Bad Request";
    status[404] = "Not Found";
    status[405] = "Method Not Allowed";
    status[413] = "Request Entity Too Large";
    status[500] = "Internal Server Error";
    status[501] = "Not Implemented";
    status[503] = "Service Unavailable";
    status[505] = "HTTP Version Not Supported";

    return status[code];
}
