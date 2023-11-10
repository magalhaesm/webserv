#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <string>
#include <sys/epoll.h>

class Server;
class ConnectionManager;

// - Representa uma conexão entre um servidor e um cliente.
// - Lida com o controle de estados da conexão e pode descartar a conexão se necessário.
class Connection
{
public:
    Connection(Server* server, ConnectionManager* manager);
    Connection(Server* server, ConnectionManager* manager, struct epoll_event* event);
    ~Connection();
    void notifyEvent();
    void close();
    int getSocket() const;
    std::string read();
    void write(const std::string& response);
    void switchMode(int op);

private:
    int m_clientSocket;
    Server* m_server;
    ConnectionManager* m_manager;
    struct epoll_event* m_event;
};

#endif // !CONNECTION_HPP
