#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>

#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "ARequestHandler.hpp"
#include "HTMLController.hpp"
#include "CGIController.hpp"
#include "ConfigSpec.hpp"

class Connection;

class Server
{
public:
    Server(const ConfigSpec& cfg);
    ~Server();

    void handleRequest(const HTTPRequest& req, HTTPResponse& res);
    void listen();
    int accept();
    int getSocket() const;
    int getClientMaxBodySize() const;

private:
    const ConfigSpec& _cfg;
    std::string _name;
    int _port;
    int _socket;
    ARequestHandler* _handler;
    std::vector<ARequestHandler*> _handlers;
    HTMLController htmlController;
    CGIController cgiController;

    int createSocket();
};

#endif // !SERVER_HPP
