#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>

class ConfigSpec;

class Server
{
public:
    Server(const ConfigSpec& cfg);
    ~Server();
    void listen();
    int getSocket() const;

private:
    std::string m_name;
    std::string m_host;
    int m_port;
    int m_socket;

    int createSocket();
};

#endif // !SERVER_HPP
