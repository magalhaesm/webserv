#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include "ConfigSpec.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

// REF: cpp -> módulo 1 -> ex 05-06

class Logger
{
public:
    static void log(HTTPRequest& req, HTTPResponse res, const ConfigSpec& cfg);

private:
    static std::string getTimeStamp();
};

#endif // !LOGGER_HPP
