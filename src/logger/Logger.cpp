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

void Logger::log(Request& req, Response& res, const ConfigSpec& cfg)
{
    std::cout << getTimeStamp() << " " << cfg.getServerName() << ": " << req.methodText() << " "
              << req.path() << " - " << res.getStatus() << " " << statusText(res.getStatus());

    if (req.method() == POST)
    {
        std::cout << " " << req.getHeader("Referer");
    }
    std::cout << std::endl;
}

void Logger::log(const std::string& message)
{
    std::cout << getTimeStamp() << " " << message << std::endl;
}
