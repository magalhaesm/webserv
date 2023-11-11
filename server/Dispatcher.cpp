#include "Dispatcher.hpp"

Dispatcher::Dispatcher()
{
}

Dispatcher::~Dispatcher()
{
    std::map<int, Connection*>::iterator it;
    for (it = active.begin(); it != active.end(); ++it)
    {
        delete it->second;
    }
}

void Dispatcher::notify(struct epoll_event* event)
{
    Connection* conn = get(event->data.fd);
    conn->notify(event);
}

Connection* Dispatcher::connect(Server* server)
{
    Connection* connection = new Connection(server, this);

    int clientSocket = connection->getSocket();
    active[clientSocket] = connection;

    return connection;
}

void Dispatcher::close(Connection* conn)
{
    std::map<int, Connection*>::iterator it = active.find(conn->getSocket());
    if (it != active.end())
    {
        delete it->second;
        active.erase(it);
    }
}

inline Connection* Dispatcher::get(int socket)
{
    std::map<int, Connection*>::iterator it = active.find(socket);
    if (it != active.end())
    {
        return it->second;
    }
    return NULL;
}
