#ifndef CONFIG_SPEC_HPP
#define CONFIG_SPEC_HPP

#include <string>

#include "Directives.hpp"

class ConfigSpec
{
public:
    ConfigSpec(Directives* directives, const std::string& location = "/");
    ConfigSpec(const ConfigSpec& src);
    ~ConfigSpec();
    ConfigSpec& operator=(const ConfigSpec& rhs);

    int getPort() const;
    const std::string& getServerName() const;
    const std::string& getIndex() const;
    const std::string getRoot() const;
    bool hasAutoindex() const;
    bool hasErrorPage(int error) const;
    const std::string& getErrorPage(int error) const;
    bool hasCGI() const;
    const std::string& getCGI() const;
    bool hasRedirect() const;
    const Redirect& getRedirect() const;
    std::string match(const std::string& path) const;
    ConfigSpec getContext(const std::string& path) const;
    std::string getLocation() const;
    int getClientBodySize() const;
    bool isMethodAllowed(const std::string& method) const;

private:
    Directives* _directives;
    std::string _location;
    std::string _empty;
};

#endif // !CONFIG_SPEC_HPP
