#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <ctime>
#include <string>

#include "Message.hpp"

class Server;
class HTTPResponse;
class EventListener;

class Connection
{
public:
    Connection(EventListener* listener, Server* server);
    ~Connection();

    bool read();
    bool write();
    bool close();
    int getID() const;
    std::time_t getLastActivityTime() const;

private:
    int _clientSocket;
    Server* _server;
    EventListener* _listener;
    std::time_t _lastActivityTime;
    std::string _raw;
    Message _msg;
    int _maxBodySize;

    void send(const std::string& response);
    void processRequest();
    void updateLastActivityTime();
};

#endif // !CONNECTION_HPP
