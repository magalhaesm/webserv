#include <ctime>
#include <iostream>

#include "Logger.hpp"
#include "HTTPConstants.hpp"

std::string Logger::getTimeStamp()
{
    const int MAXLEN = 100;
    char timestamp[MAXLEN];

    std::time_t currentTime = std::time(0);
    std::strftime(timestamp, MAXLEN, "[%Y-%m-%d %H:%M:%S]", std::localtime(&currentTime));

    return timestamp;
}

void Logger::log(HTTPRequest& req, HTTPResponse res, const ConfigSpec& cfg)
{
    std::cout << getTimeStamp() << " " << cfg.getServerName() << ": " << req.methodText() << " "
              << req.path() << " - " << res.getStatus() << " " << statusText(res.getStatus())
              << std::endl;
}
