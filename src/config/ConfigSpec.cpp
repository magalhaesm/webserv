#include "ConfigSpec.hpp"
#include "Directives.hpp"

ConfigSpec::ConfigSpec(Directives* directives)
    : _directives(directives)
{
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
    return _directives->redirect.code && !_directives->redirect.url.empty();
}

const Redirect& ConfigSpec::getRedirect() const
{
    return _directives->redirect;
}

std::string ConfigSpec::getLocation(const std::string& path) const
{
    Locations::iterator it = _directives->locations.begin();
    for (; it != _directives->locations.end(); ++it)
    {
        if (path.find(it->first) != std::string::npos)
        {
            return it->first;
        }
    }
    return _empty;
}

ConfigSpec ConfigSpec::getContext(const std::string& path) const
{
    Directives* ctx = &_directives->locations.at(path);
    ctx->listen = _directives->listen;
    ctx->server_name = _directives->server_name;
    if (ctx->root.empty())
    {
        ctx->root = _directives->root;
    }
    if (ctx->index.empty())
    {
        ctx->index = _directives->index;
    }
    return ConfigSpec(ctx);
}

int ConfigSpec::getClientBodySize() const
{
    return _directives->client_max_body_size;
}

bool ConfigSpec::isMethodAllowed(const std::string& method) const
{
    return _directives->limit_except.empty() || _directives->limit_except.count(method);
}
