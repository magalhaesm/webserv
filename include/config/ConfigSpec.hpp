#ifndef CONFIG_SPEC_HPP
#define CONFIG_SPEC_HPP

#include <string>

#include "Directives.hpp"

class ConfigSpec
{
public:
    ConfigSpec(Directives* directives);
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
    std::string getLocation(const std::string& path) const;
    ConfigSpec getContext(const std::string& path) const;
    int getClientBodySize() const;
    bool allow(const std::string& method) const;

private:
    Directives* _directives;
    std::string _empty;
};

#endif // !CONFIG_SPEC_HPP
