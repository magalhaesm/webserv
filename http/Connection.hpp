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
    void setPersistent(bool persistent);

private:
    int m_clientSocket;
    Server* m_server;
    EventListener* m_listener;
    std::time_t m_lastActivityTime;
    bool m_persistent;
    std::string m_raw;
    Message m_msg;

    void send(const std::string& response);
    void processRequest();
    void updateLastActivityTime();
};

#endif // !CONNECTION_HPP
