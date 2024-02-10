#include <vector>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>

#include "Connection.hpp"
#include "EventListener.hpp"

const int TIMEOUT_SEC = 5;
const int TIMEOUT_MS = 3000;

bool g_running = true;

static void sigIntHandler(int);
static void setSignalHandler();

EventListener::EventListener()
    : m_epfd(epoll_create(1))
{
}

EventListener::~EventListener()
{
    std::map<int, Connection*>::iterator it;
    for (it = m_active.begin(); it != m_active.end(); ++it)
    {
        delete it->second;
    }
    ::close(m_epfd);
}

void EventListener::subscribe(Server* server)
{
    int socket = server->getSocket();
    m_servers[socket] = server;

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = socket;
    epoll_ctl(m_epfd, EPOLL_CTL_ADD, socket, &ev);
}

void EventListener::start()
{
    startServers();
    setSignalHandler();

    while (g_running)
    {
        int ready = epoll_wait(m_epfd, m_events, MAX_EVENTS, TIMEOUT_MS);
        for (int idx = 0; idx < ready; ++idx)
        {
            handleEvent(&m_events[idx]);
        }
        checkTimeout(TIMEOUT_SEC);
    }
}

void EventListener::close(Connection* conn)
{
    int id = conn->getID();
    delete m_active.at(id);
    m_active.erase(id);
}

inline void EventListener::startServers()
{
    std::map<int, Server*>::iterator it;
    for (it = m_servers.begin(); it != m_servers.end(); ++it)
    {
        Server* server = it->second;
        server->listen();
    }
}

inline void EventListener::handleEvent(struct epoll_event* ev)
{
    if (establishConnection(ev->data.fd))
    {
        return;
    }

    Connection* conn = m_active.at(ev->data.fd);

    if ((ev->events & EPOLLIN) == EPOLLIN)
    {
        update(conn->read(), ev, EPOLLOUT);
    }
    else if ((ev->events & EPOLLOUT) == EPOLLOUT)
    {
        update(conn->write(), ev, EPOLLIN);
    }
}

bool EventListener::establishConnection(int socket)
{
    std::map<int, Server*>::const_iterator it = m_servers.find(socket);
    if (it != m_servers.end())
    {
        Server* server = it->second;
        Connection* conn = new Connection(this, server);

        int client = conn->getID();
        m_active[client] = conn;

        struct epoll_event ev;
        ev.events = EPOLLIN | EPOLLOUT;
        ev.data.fd = client;
        epoll_ctl(m_epfd, EPOLL_CTL_ADD, client, &ev);

        return true;
    }
    return false;
}

inline void EventListener::checkTimeout(std::time_t threshold)
{
    std::vector<Connection*> expired;
    std::map<int, Connection*>::iterator it;

    for (it = m_active.begin(); it != m_active.end(); ++it)
    {
        Connection* conn = it->second;
        std::time_t elapsedTime = time(NULL) - conn->getLastActivityTime();

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

inline void EventListener::update(bool change, struct epoll_event* ev, uint32_t events)
{
    if (change == false)
    {
        return;
    }
    ev->events = events;
    epoll_ctl(m_epfd, EPOLL_CTL_MOD, ev->data.fd, ev);
}

void sigIntHandler(int)
{
    g_running = false;
    std::cerr << "signal: interrupt" << std::endl;
}

void setSignalHandler()
{
    struct sigaction sa;
    sa.sa_handler = sigIntHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
}
