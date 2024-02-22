#include <cstdlib>
#include <iostream>

#include "ConfigSpec.hpp"

ConfigSpec::ConfigSpec(void)
{
    return;
}

// ConfigSpec::ConfigSpec(const std::map<std::string, std::vector<std::string> >& directives,
//                         const std::map<std::string, std::map<std::string, std::string> >&
//                         locationBlocks) : directives(directives), locationBlocks(locationBlocks)
//                         {}

ConfigSpec::ConfigSpec(ConfigSpec const& src)
{
    *this = src;
}

ConfigSpec::~ConfigSpec(void)
{
    return;
}

ConfigSpec& ConfigSpec::operator=(const ConfigSpec& rhs)
{
    // Check for self-assignment avoiding stack overflow
    if (this != &rhs)
    {
        this->_directives = rhs._directives;
        this->_locationBlocks = rhs._locationBlocks;
    }
    return *this;
}

void ConfigSpec::setDirectives(const std::map<std::string, std::vector<std::string> >& directives)
{
    this->_directives = directives;
}

void ConfigSpec::setLocationBlocks(
    const std::map<std::string, std::map<std::string, std::string> >& locationBlocks)
{
    this->_locationBlocks = locationBlocks;
}

/* GETTERS */

int ConfigSpec::getPort() const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it =
        _directives.find("listen");

    int port = std::atoi(it->second[0].c_str());

    if (it != _directives.end())
    {
        if (port < 1 || port > 65535)
            throw std::invalid_argument("Usage: invalid port");
        return port;
    }
    else
        return -1;
}

std::string ConfigSpec::getServerName() const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it =
        _directives.find("server_name");

    if (it != _directives.end())
    {
        if (it->second[0].empty())
            throw std::invalid_argument("Usage: Expect argument for server name");
        return it->second[0];
    }
    else
        return " ";
}

std::string ConfigSpec::getIndex() const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = _directives.find("index");

    if (it != _directives.end())
    {
        if (it->second[0].empty())
            throw std::invalid_argument("Usage: Expect argument for index");
        return it->second[0];
    }
    else
        return " ";
}

std::string ConfigSpec::getRoot() const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = _directives.find("root");

    if (it != _directives.end())
    {
        if (it->second[0].empty())
            throw std::invalid_argument("Usage: Expect argument for root");
        return it->second[0];
    }
    else
        return " ";

    if (it != _directives.end())
        return it->second[0];
    else
        return " ";
}

std::string ConfigSpec::getAutoindex() const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it =
        _directives.find("autoindex");

    if (it != _directives.end())
    {
        if (it->second[0].empty())
            throw std::invalid_argument("Usage: Expect 1 argument for autoindex");
        if (it->second[0] != "on" && it->second[0] != "off")
            throw std::invalid_argument(
                "Usage: Invalid argument for 'autoindex' - expect 'on' or 'off'");

        return it->second[0];
    }
    else
        return " ";
}

std::vector<std::string> ConfigSpec::getErrorPage() const
{
    std::vector<std::string> errorPage;

    std::map<std::string, std::vector<std::string> >::const_iterator it =
        _directives.find("error_page");

    if (it != _directives.end())
    {
        if (it->second[0].empty())
            throw std::invalid_argument("Usage: Expect 1 argument for error page");

        for (size_t i = 0; i < it->second.size(); ++i)
            errorPage.push_back(it->second[i]);
        return errorPage;
    }
    else
        return std::vector<std::string>();
}

std::string ConfigSpec::getCgi() const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = _directives.find("cgi");

    if (it != _directives.end())
    {
        if (it->second.size() == 1 && it->second[0] == ".py python3")
            return it->second[0];
        else
            throw std::invalid_argument("Usage: Invalid argument for 'cgi' - expect '.py python3'");
    }
    else
        return " ";
}

std::string ConfigSpec::getRedirect() const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it =
        _directives.find("redirect");

    if (it != _directives.end())
    {
        if (it->second.size() == 1 && (!it->second[0].empty()))
            return it->second[0];
        else
            throw std::invalid_argument("Usage: expect 'redirect' argument");
    }
    else
        return " ";
}

/* DEBUG */

void ConfigSpec::printParsedDirectives(void) const
{
    std::cout << "Parsed Directives:\n" << std::endl;
    std::map<std::string, std::vector<std::string> >::const_iterator it;
    for (it = _directives.begin(); it != _directives.end(); ++it)
    {
        std::cout << "  " << it->first << ":" << std::endl;
        std::vector<std::string>::const_iterator vecIt = it->second.begin();
        for (; vecIt != it->second.end(); ++vecIt)
            std::cout << "    " << *vecIt << std::endl;
    }
}

void ConfigSpec::printParsedLocationBlocks(void) const
{
    std::cout << "Parsed Location Blocks:\n" << std::endl;
    std::map<std::string, std::map<std::string, std::string> >::const_iterator it;
    for (it = _locationBlocks.begin(); it != _locationBlocks.end(); ++it)
    {
        std::cout << "Location: " << it->first << std::endl;

        const std::map<std::string, std::string>& directives = it->second;
        for (std::map<std::string, std::string>::const_iterator innerIt = directives.begin();
             innerIt != directives.end();
             ++innerIt)
            std::cout << "  " << innerIt->first << ": " << innerIt->second << std::endl;
        std::cout << std::endl;
    }
}
