#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>

#include "HTTPParser.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "HTMLController.hpp"
#include "CGIController.hpp"

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

    void handleRequest(const HTTPRequest* request, HTTPResponse* response);
    void listen();
    int accept();
    int getSocket() const;
    HTTPParser& getParser();

private:
    std::string m_name;
    std::string m_host;
    int m_port;
    int m_socket;
    HTMLController htmlController;
    CGIController cgiController;
    HTTPParser m_parser;

    int createSocket();
};

#endif // !SERVER_HPP
