#include "ConfigSpec.hpp"
#include "Directives.hpp"

ConfigSpec::ConfigSpec(Directives& directives, const ConfigSpec* parent)
    : _directives(directives)
    , _parent(parent)
{
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
    if (!_directives.listen)
    {
        return _parent->_directives.listen;
    }
    return _directives.listen;
}

const std::string& ConfigSpec::getServerName() const
{
    if (_directives.server_name.empty())
    {
        return _parent->_directives.server_name;
    }
    return _directives.server_name;
}

const std::string& ConfigSpec::getIndex() const
{
    return _directives.index;
}

const std::string& ConfigSpec::getRoot() const
{
    return _directives.root;
}

bool ConfigSpec::hasAutoindex() const
{
    return _directives.autoindex;
}

bool ConfigSpec::hasErrorPage(int error) const
{
    return _directives.error_page.count(error);
}

const std::string& ConfigSpec::getErrorPage(int error) const
{
    return _directives.error_page.at(error);
}

bool ConfigSpec::hasCGI() const
{
    return !_directives.cgi.empty();
}

const std::string& ConfigSpec::getCGI() const
{
    return _directives.cgi;
}

bool ConfigSpec::hasRedirect() const
{
    return _directives.redirect.first && !_directives.redirect.second.empty();
}

const Redirect& ConfigSpec::getRedirect() const
{
    return _directives.redirect;
}

bool ConfigSpec::hasLocation(const std::string& location) const
{
    return _directives.locations.count(location);
}

ConfigSpec ConfigSpec::getLocation(const std::string& location) const
{
    return ConfigSpec(_directives.locations.at(location), this);
}
