#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <ctime>
#include <string>
#include <sys/epoll.h>

class Server;
class Dispatcher;
class HTTPRequest;
class HTTPResponse;

class Connection
{
public:
    Connection(Server* server, Dispatcher* dispatcher);
    ~Connection();

    void notify(struct epoll_event* event);
    std::string read();
    void write(HTTPResponse* response);
    int getSocket() const;
    std::time_t getLastActivity() const;

    const HTTPRequest* request();
    HTTPResponse* response();

private:
    int m_clientSocket;
    Server* m_server;
    Dispatcher* m_dispatcher;
    HTTPRequest* m_request;
    HTTPResponse* m_response;
    std::time_t m_timeOfLastActivity;

    void close();
};

#endif // !CONNECTION_HPP
