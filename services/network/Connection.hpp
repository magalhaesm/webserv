#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <ctime>
#include <string>
#include <sys/epoll.h>

class Server;
class HTTPRequest;
class HTTPResponse;
class EventListener;
class HTTPParser;

class Connection
{
public:
    Connection(EventListener* listener, Server* server);
    ~Connection();

    bool read();
    bool write();
    bool close();
    int getSocket() const;
    std::time_t getLastActivityTime() const;

    HTTPResponse* response();

private:
    int m_clientSocket;
    Server* m_server;
    EventListener* m_listener;
    HTTPRequest* m_request;
    HTTPResponse* m_response;
    HTTPParser& m_parser;
    std::time_t m_lastActivityTime;
    std::string m_buffer;

    void updateLastActivityTime();
};

#endif // !CONNECTION_HPP
