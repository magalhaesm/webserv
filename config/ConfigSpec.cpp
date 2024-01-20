#include "ConfigSpec.hpp"
#include <iostream>
#include <sstream>

ConfigSpec::ConfigSpec(void)
{
    return;
}

// ConfigSpec::ConfigSpec(const std::map<std::string, std::vector<std::string> >& directives,
//                         const std::map<std::string, std::map<std::string, std::string> >& locationBlocks) : directives(directives), locationBlocks(locationBlocks) {}

ConfigSpec::ConfigSpec(ConfigSpec const &src)
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


void ConfigSpec::setDirectives(const std::map<std::string, std::vector<std::string> > &directives)
{
    this->_directives = directives;
}


void ConfigSpec::setLocationBlocks(const std::map<std::string, std::map<std::string, std::string> > &locationBlocks)
{
    this->_locationBlocks = locationBlocks;
}


std::vector<std::string> ConfigSpec::getDirectiveValues(const std::string& directiveName)
{
    std::vector<std::string> values;
    std::map<std::string, std::vector<std::string> >::iterator it = _directives.find(directiveName);
    
    if (it != _directives.end())
    {
        values = it->second;
        return values;
    }
    else
        return std::vector<std::string>();
};

int ConfigSpec::getPort() const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = _directives.find("listen");

    if (it != _directives.end())
        return std::atoi(it->second[0].c_str());
    else
        return -1;
}

std::vector<std::string> ConfigSpec::getServerName() const
{
    std::vector<std::string> listenValues;

    std::map<std::string, std::vector<std::string> >::const_iterator it = _directives.find("server_name");
    
    if (it != _directives.end())
    {
        for(size_t i = 0; i < it->second.size(); ++i)
            listenValues.push_back(it->second[i]);
        return listenValues;
    }
    else
        return std::vector<std::string>();
}

std::string ConfigSpec::getIndex() const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = _directives.find("index");

    if (it != _directives.end())
        return it->second[0];
    else
        return " ";
}

std::string ConfigSpec::getRoot() const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = _directives.find("root");

    if (it != _directives.end())
        return it->second[0];
    else
        return " ";
}

std::string ConfigSpec::getAutoindex() const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = _directives.find("autoindex");

    if (it != _directives.end())
        return it->second[0];
    else
        return " ";
}

std::vector<std::string> ConfigSpec::getErrorPage() const
{
    std::vector<std::string> errorPage;

    std::map<std::string, std::vector<std::string> >::const_iterator it = _directives.find("error_page");
    
    if (it != _directives.end())
    {
        for(size_t i = 0; i < it->second.size(); ++i)
            errorPage.push_back(it->second[i]);
        return errorPage;
    }
    else
        return std::vector<std::string>();
}

std::vector<std::string> ConfigSpec::getCgi() const
{
    std::vector<std::string> cgi;

    std::map<std::string, std::vector<std::string> >::const_iterator it = _directives.find("cgi");
    
    if (it != _directives.end())
    {
        for(size_t i = 0; i < it->second.size(); ++i)
            cgi.push_back(it->second[i]);
        return cgi;
    }
    else
        return std::vector<std::string>();
}

std::vector<std::string> ConfigSpec::getRedirect() const
{
    std::vector<std::string>redirect;

    std::map<std::string, std::vector<std::string> >::const_iterator it = _directives.find("redirect");
    
    if (it != _directives.end())
    {
        for(size_t i = 0; i < it->second.size(); ++i)
            redirect.push_back(it->second[i]);
        return redirect;
    }
    else
        return std::vector<std::string>();
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
        for ( ; vecIt != it->second.end(); ++vecIt)
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
        for (std::map<std::string, std::string>::const_iterator innerIt = directives.begin(); innerIt != directives.end(); ++innerIt)
            std::cout << "  " << innerIt->first << ": " << innerIt->second << std::endl;
        std::cout << std::endl;
    }    
}
