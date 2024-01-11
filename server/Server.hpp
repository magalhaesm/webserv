#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <string>

class Connection;

class ConfigSpec
{
public:
    ConfigSpec(int port, std::string name)
        : m_port(port)
        , m_name(name)
    {
    }

    int getPort() const
    {
        return m_port;
    }
    std::string getServerName() const
    {
        return m_name;
    }
    std::string getHostName() const
    {
        return "www.42sp.com.br";
    }
    int getBodySizeLimit() const
    {
        return 1024;
    }

private:
    int m_port;
    std::string m_name;
};

class Server
{
public:
    Server(const ConfigSpec& cfg);
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
