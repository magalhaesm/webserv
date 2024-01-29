#include <ctime>
#include <vector>

#include "Dispatcher.hpp"

Dispatcher::Dispatcher()
{
}

Dispatcher::~Dispatcher()
{
    std::map<int, Connection*>::iterator it;
    for (it = m_active.begin(); it != m_active.end(); ++it)
    {
        delete it->second;
    }
}

void Dispatcher::notify(struct epoll_event* event)
{
    Connection* conn = getConnection(event->data.fd);
    conn->notify(event);
}

void Dispatcher::checkTimeout(std::time_t threshold)
{
    std::vector<Connection*> expired;
    std::map<int, Connection*>::iterator it = m_active.begin();

    for (; it != m_active.end(); ++it)
    {
        Connection* conn = it->second;
        std::time_t elapsedTime = time(NULL) - conn->getLastActivity();

        if (elapsedTime > threshold)
        {
            expired.push_back(conn);
        }
    }
    for (size_t i = 0; i < expired.size(); ++i)
    {
        close(expired[i]);
    }
}

Connection* Dispatcher::connect(Server* server)
{
    Connection* connection = new Connection(server, this);

    int clientSocket = connection->getSocket();
    m_active[clientSocket] = connection;

    return connection;
}

void Dispatcher::close(Connection* conn)
{
    std::map<int, Connection*>::iterator it = m_active.find(conn->getSocket());
    if (it != m_active.end())
    {
        delete it->second;
        m_active.erase(it);
    }
}

inline Connection* Dispatcher::getConnection(int socket)
{
    std::map<int, Connection*>::iterator it = m_active.find(socket);
    if (it != m_active.end())
    {
        return it->second;
    }
    return NULL;
}
