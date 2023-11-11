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

void EventListener::subscribe(Server* target)
{
    int socket = target->getSocket();
    m_servers[socket] = target;
    watch(socket);
}

void EventListener::start()
{
    startServers();
    setSignalHandler();
    while (g_running)
    {
        int eventCount = epoll_wait(m_epollFd, m_events, MAX_EVENTS, -1);
        for (int idx = 0; idx < eventCount; ++idx)
        {
            int socket = m_events[idx].data.fd;
            Server* server = findReceiver(socket);
            if (server)
            {
                acceptConnection(server);
            }
            else
            {
                // m_dispatcher.notify(m_events[idx]);
                Connection* conn = m_manager.getConnection(socket);
                conn->notify(&m_events[idx]);
            }
        }
    }
}

void EventListener::acceptConnection(Server* server)
{
    Connection* client = m_manager.connect(server);
    watch(client->getSocket());
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

void EventListener::watch(int socket)
{
    struct epoll_event m_event;
    m_event.events = EPOLLIN | EPOLLET;
    m_event.data.fd = socket;
    epoll_ctl(m_epollFd, EPOLL_CTL_ADD, socket, &m_event);
}

inline Server* EventListener::findReceiver(int socket)
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
