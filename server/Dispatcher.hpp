#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include <map>
#include <sys/epoll.h>

#include "Server.hpp"
#include "Connection.hpp"

// Responsável por abrir, fechar e notificar eventos em conexões
class Dispatcher
{
public:
    Dispatcher();
    ~Dispatcher();
    Connection* connect(Server* server);
    void close(Connection* connection);
    void notify(struct epoll_event* event);

private:
    std::map<int, Connection*> active;

    Connection* get(int socket);
};

#endif // !DISPATCHER_HPP
