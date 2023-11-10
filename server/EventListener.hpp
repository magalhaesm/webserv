#ifndef EVENT_LISTENER_HPP
#define EVENT_LISTENER_HPP

#include <map>
#include <vector>
#include <sys/epoll.h>

#include "Server.hpp"
#include "Connection.hpp"
#include "ConnectionManager.hpp"

// - Gerencia o único `poll()` para todas as operações de I/O no servidor.
// - Realiza verificações de leitura e escrita simultaneamente.
// EventListener que monitorar eventos no sockets de conexão.
// Quando um evento ocorre é função da classe criar uma nova conexão entre
// cliente e servidor e se a conexão já existe, a classe notifica o servidor
// de uma ação do cliente.
class EventListener
{
public:
    EventListener();
    ~EventListener();
    void subscribe(Server* target);
    void start();

private:
    static const int MAX_EVENTS = 10;

    int m_epollFd;
    struct epoll_event m_events[MAX_EVENTS];
    std::map<int, Server*> m_servers;
    ConnectionManager m_manager;

    void startServers();
    void onIncomingConnection(Server* server);
    void onDataReceived(int socket);
    void watchSocket(int socket);
    Server* findServerBySocket(int socket);
    void test(struct epoll_event* event);
};

#endif // !EVENT_LISTENER_HPP
