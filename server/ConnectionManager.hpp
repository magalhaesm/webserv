#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP

#include <map>

#include "Server.hpp"
#include "Connection.hpp"

// Responsável apenas iniciar e fechar conexões
class ConnectionManager
{
public:
    ConnectionManager();
    ~ConnectionManager();
    Connection* connect(Server* server);
    void close(Connection* connection);
    Connection* getConnection(int socket);

private:
    std::map<int, Connection*> active;
};

#endif // !CONNECTION_MANAGER_HPP
