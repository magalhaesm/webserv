#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <ctime>
#include <string>
#include <sys/epoll.h>

class Server;
class HTTPRequest;
class HTTPResponse;
class EventListener;

class Connection
{
public:
    Connection(Server* server, EventListener* listener);
    ~Connection();

    bool read();
    bool write();
    void close();
    int getSocket() const;
    std::time_t getLastActivity() const;

    const HTTPRequest* request();
    HTTPResponse* response();

private:
    int m_clientSocket;
    Server* m_server;
    HTTPRequest* m_request;
    HTTPResponse* m_response;
    EventListener* m_listener;
    std::time_t m_timeOfLastActivity;
    std::string m_readBuffer;

    void updateLastActivity();
};

#endif // !CONNECTION_HPP
