#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>

#include "Request.hpp"
#include "Response.hpp"
#include "ARequestHandler.hpp"
#include "ConfigSpec.hpp"

class Connection;

class Server
{
public:
    Server(const ConfigSpec& cfg);
    ~Server();

    void handleRequest(Request& req, Response& res);
    void listen();
    int accept();
    int getSocket() const;
    int getClientMaxBodySize() const;

private:
    const ConfigSpec& _cfg;
    std::string _name;
    int _port;
    int _socket;
    ARequestHandler* _initHandler;
    std::vector<ARequestHandler*> _handlers;

    int createSocket();
    void setupHandlers();
};

#endif // !SERVER_HPP
