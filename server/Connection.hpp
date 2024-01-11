#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <string>
#include <sys/epoll.h>

class Server;
class Dispatcher;
class HTTPRequest;
class HTTPResponse;

// - Representa uma conexão entre um servidor e um cliente.
// - Lida com o controle de estados da conexão e pode descartar a conexão se necessário.
class Connection
{
public:
    Connection(Server* server, Dispatcher* dispatcher);
    ~Connection();
    void notify(struct epoll_event* event);
    std::string read();
    void write(const std::string& response);
    void close();
    int getSocket() const;
    HTTPRequest* request();
    HTTPResponse* response();

private:
    int m_clientSocket;
    Server* m_server;
    Dispatcher* m_dispatcher;
    HTTPRequest* m_request;
    HTTPResponse* m_response;
};

#endif // !CONNECTION_HPP
