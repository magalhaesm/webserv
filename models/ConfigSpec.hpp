#ifndef CONFIG_SPEC_HPP
#define CONFIG_SPEC_HPP

#include <string>

class ConfigSpec
{
public:
    int getPort() const;
    std::string getServerName() const;
    std::string getHostName() const;
    int getBodySizeLimit() const;

private:
    std::string m_serverName;
    std::string m_host;
    int m_port;
    int m_limitBodySize;
};

#endif // !CONFIG_SPEC_HPP
