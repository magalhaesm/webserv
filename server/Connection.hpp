#ifndef CONNECTION_HPP
#define CONNECTION_HPP

class Server;
class ConnectionManager;

// - Representa uma conexão entre um servidor e um cliente.
// - Lida com o controle de estados da conexão e pode descartar a conexão se necessário.
class Connection
{
public:
    Connection(Server* server, ConnectionManager* manager);
    ~Connection();
    void notifyServer();
    void close();
    int getSocket() const;
    void switchMode(int op);

private:
    int m_clientSocket;
    Server* m_server;
    ConnectionManager* m_manager;
};

#endif // !CONNECTION_HPP
