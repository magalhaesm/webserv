#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

class Server;
class ConnectionManager;

#include "Connection.hpp"
#include "ConnectionManager.hpp"

// TODO: deve ser capaz de se incluir e remover da lista de interesse
Connection::Connection(Server* server, ConnectionManager* manager)
    : m_server(server)
    , m_manager(manager)
{
    m_clientSocket = accept(server->getSocket(), NULL, NULL);
}

Connection::~Connection()
{
    ::close(m_clientSocket);
}

void Connection::notifyServer()
{
    m_server->handleIncomingData(this);
}

void Connection::close()
{
    m_manager->close(this);
}

int Connection::getSocket() const
{
    return m_clientSocket;
}
