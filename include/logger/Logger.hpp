#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

#include "Request.hpp"
#include "Response.hpp"
#include "ConfigSpec.hpp"

class Logger
{
public:
    static void log(Request& req, Response& res, const ConfigSpec& cfg);
    static void log(const std::string& message);

private:
    static std::string getTimeStamp();
};

#endif // !LOGGER_HPP
