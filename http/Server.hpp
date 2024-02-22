#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>

#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "HTMLController.hpp"
#include "CGIController.hpp"
#include "ConfigSpec.hpp"

class Connection;

class Server
{
public:
    Server(const ConfigSpec& cfg);
    ~Server();

    void handleRequest(const HTTPRequest& request, HTTPResponse& response);
    void listen();
    int accept();
    int getSocket() const;

private:
    std::string m_name;
    int m_port;
    int m_socket;
    HTMLController htmlController;
    CGIController cgiController;

    int createSocket();
};

#endif // !SERVER_HPP
