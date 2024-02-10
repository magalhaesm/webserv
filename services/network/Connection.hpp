#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <ctime>
#include <string>
#include <sys/epoll.h>

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
    void send(const std::string& response);
    int getID() const;
    std::time_t getLastActivityTime() const;
    void setPersistent(bool persistent);

    HTTPResponse* response();

private:
    int m_clientSocket;
    Server* m_server;
    EventListener* m_listener;
    std::time_t m_lastActivityTime;
    std::string m_buffer;
    bool m_persistent;

    void updateLastActivityTime();
};

#endif // !CONNECTION_HPP
