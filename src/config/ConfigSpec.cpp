#include <stdexcept>

#include "ConfigSpec.hpp"
#include "Directives.hpp"

ConfigSpec::ConfigSpec(Directives* directives, const ConfigSpec* parent)
    : _directives(directives)
    , _parent(parent)
{
    if (_parent)
    {
        _directives->root = _parent->_directives->root;
        _directives->listen = _parent->_directives->listen;
        _directives->server_name = _parent->_directives->server_name;
    }

    if (_directives->index.empty())
    {
        _directives->index = "index.html";
    }
}

ConfigSpec::ConfigSpec(const ConfigSpec& src)
    : _directives(src._directives)
{
}

ConfigSpec::~ConfigSpec()
{
}

ConfigSpec& ConfigSpec::operator=(const ConfigSpec& rhs)
{
    if (this != &rhs)
    {
        this->_directives = rhs._directives;
        this->_parent = rhs._parent;
    }
    return *this;
}

int ConfigSpec::getPort() const
{
    return _directives->listen;
}

const std::string& ConfigSpec::getServerName() const
{
    return _directives->server_name;
}

const std::string& ConfigSpec::getIndex() const
{
    return _directives->index;
}

const std::string ConfigSpec::getRoot() const
{
    if (_directives->root.empty())
    {
        return "./";
    }
    return _directives->root + "/";
}

bool ConfigSpec::hasAutoindex() const
{
    return _directives->autoindex;
}

bool ConfigSpec::hasErrorPage(int error) const
{
    return _directives->error_page.count(error);
}

const std::string& ConfigSpec::getErrorPage(int error) const
{
    return _directives->error_page.at(error);
}

bool ConfigSpec::hasCGI() const
{
    return !_directives->cgi.empty();
}

const std::string& ConfigSpec::getCGI() const
{
    return _directives->cgi;
}

bool ConfigSpec::hasRedirect() const
{
    return _directives->redirect.first && !_directives->redirect.second.empty();
}

const Redirect& ConfigSpec::getRedirect() const
{
    return _directives->redirect;
}

bool ConfigSpec::hasLocation(const std::string& location) const
{
    Locations::iterator it = _directives->locations.begin();
    for (; it != _directives->locations.end(); ++it)
    {
        if (location.find(it->first) != std::string::npos)
        {
            return true;
        }
    }
    return false;
}

ConfigSpec ConfigSpec::getLocation(const std::string& location) const
{
    Locations::iterator it = _directives->locations.begin();
    for (; it != _directives->locations.end(); ++it)
    {
        if (location.find(it->first) != std::string::npos)
        {
            return ConfigSpec(&it->second, this);
        }
    }
    throw std::runtime_error("location not found");
}

int ConfigSpec::getClientBodySize() const
{
    return _directives->client_max_body_size;
}

bool ConfigSpec::allow(const std::string& method) const
{
    return _directives->limit_except.empty() || _directives->limit_except.count(method);
}
