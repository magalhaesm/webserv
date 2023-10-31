#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include <string>

class ServerConfig
{
public:
    int getPort() const;
    std::string getServerName() const;
    std::string getHostName() const;
    int getBodySizeLimit() const;

private:
    std::string server_name;
    std::string host;
    int port;
    int client_limit_body_size;
};

#endif // !SERVER_CONFIG_HPP
