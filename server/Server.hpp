#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <string>
#include "ConfigParser.hpp"

class Connection;

class Server
{
public:
    Server(ConfigSpec& cfg);
    ~Server();
    void listen();
    int accept();
    bool read(Connection* conn);
    bool write(Connection* conn);
    int getSocket() const;

private:
    std::string m_name;
    std::string m_host;
    int m_port;
    int m_socket;

    int createSocket();
};

#endif // !SERVER_HPP
