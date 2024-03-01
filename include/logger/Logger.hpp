#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include "ConfigSpec.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

class Logger
{
public:
    static void log(HTTPRequest& req, HTTPResponse& res, const ConfigSpec& cfg);
    static void log(const std::string& message);

private:
    static std::string getTimeStamp();
};

#endif // !LOGGER_HPP
