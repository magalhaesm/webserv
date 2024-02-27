#ifndef CONFIG_SPEC_HPP
#define CONFIG_SPEC_HPP

#include <string>

#include "Directives.hpp"

class ConfigSpec
{
public:
    ConfigSpec(Directives& directives, const ConfigSpec* parent = NULL);
    ConfigSpec(const ConfigSpec& src);
    ~ConfigSpec();
    ConfigSpec& operator=(const ConfigSpec& rhs);

    int getPort() const;
    const std::string& getServerName() const;
    const std::string& getIndex() const;
    const std::string& getRoot() const;
    bool hasAutoindex() const;
    bool hasErrorPage(int error) const;
    const std::string& getErrorPage(int error) const;
    bool hasCGI() const;
    const std::string& getCGI() const;
    bool hasRedirect() const;
    const Redirect& getRedirect() const;
    bool hasLocation(const std::string& location) const;
    ConfigSpec getLocation(const std::string& location) const;
    int getClientBodySize() const;

private:
    Directives& _directives;
    const ConfigSpec* _parent;
    std::string _empty;
};

#endif // !CONFIG_SPEC_HPP
