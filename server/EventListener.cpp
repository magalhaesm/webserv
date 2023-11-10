#include <errno.h>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <csignal>

#include "Connection.hpp"
#include "EventListener.hpp"

bool g_running = true;

static void sigIntHandler(int);
static void setSignalHandler();

EventListener::EventListener()
    : m_epollFd(epoll_create(1))
{
}

EventListener::~EventListener()
{
    close(m_epollFd);
}

// TODO: iniciar um socket aqui
void EventListener::subscribe(Server* target)
{
    int socket = target->getSocket();
    m_servers[socket] = target;
    watchSocket(socket);
}

void EventListener::test(struct epoll_event* event)
{
    std::cout << "EVENT: " << event->data.fd << std::endl;
}

class Dispatcher
{
    // private:
    //     void* connections[SOMAXCONN];
};

// toda leitura/escrita deve ser monitorada
void EventListener::start()
{
    startServers();
    setSignalHandler();
    while (g_running)
    {
        int eventCount = epoll_wait(m_epollFd, m_events, MAX_EVENTS, -1);
        for (int idx = 0; idx < eventCount; ++idx)
        {
            // Se a conexão ainda não existe, cria-se uma
            int socket = m_events[idx].data.fd;
            Server* server = findServerBySocket(socket);
            if (server)
            {
                // struct epoll_event event;
                Connection* client = m_manager.connect(server);
                watchSocket(client->getSocket());
            }
            // se existe, trata-se
            else
            {
                Connection* conn = m_manager.getConnection(socket);
                conn->notifyServer();

                // TODO:
                // conn->onRead()
            }
        }
    }
}

void EventListener::startServers()
{
    std::map<int, Server*>::iterator it;
    for (it = m_servers.begin(); it != m_servers.end(); ++it)
    {
        Server* server = it->second;
        server->listen();
    }
}

inline void EventListener::onIncomingConnection(Server* server)
{
    int clientSocket = m_manager.connect(server)->getSocket();
    watchSocket(clientSocket);
}

inline void EventListener::onDataReceived(int socket)
{
    m_manager.getConnection(socket)->notifyServer();
}

void EventListener::watchSocket(int socket)
{
    struct epoll_event m_event;
    m_event.events = EPOLLIN | EPOLLET;
    m_event.data.fd = socket;
    epoll_ctl(m_epollFd, EPOLL_CTL_ADD, socket, &m_event);
}

inline Server* EventListener::findServerBySocket(int socket)
{
    std::map<int, Server*>::iterator it = m_servers.find(socket);
    if (it != m_servers.end())
    {
        return it->second;
    }
    return NULL;
}

void sigIntHandler(int)
{
    g_running = false;
}

void setSignalHandler()
{
    struct sigaction sa;
    sa.sa_handler = sigIntHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
}
