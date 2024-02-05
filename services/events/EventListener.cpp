#include <vector>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>

#include "Connection.hpp"
#include "EventListener.hpp"

const int TIMEOUT_SEC = 15;
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
    int connID = conn->getSocket();
    delete m_active.at(connID);
    m_active.erase(connID);
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
    if (stablishConnection(ev->data.fd))
    {
        return;
    }

    Connection* conn = m_active.at(ev->data.fd);

    if (ev->events & EPOLLIN)
    {
        update(conn->read(), ev, ev->events | EPOLLOUT);
    }
    else if (ev->events & EPOLLOUT)
    {
        update(conn->write(), ev, ev->events & ~EPOLLOUT);
    }
    else if (ev->events & (EPOLLRDHUP | EPOLLERR))
    {
        conn->close();
    }
}

bool EventListener::stablishConnection(int socket)
{
    std::map<int, Server*>::const_iterator it = m_servers.find(socket);
    if (it != m_servers.end())
    {
        Server* server = it->second;
        Connection* conn = new Connection(server, this);

        int client = conn->getSocket();
        m_active[client] = conn;

        struct epoll_event ev;
        ev.events = EPOLLIN | EPOLLRDHUP | EPOLLERR | EPOLLET;
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

void EventListener::update(bool change, struct epoll_event* ev, uint32_t events)
{
    if (change)
    {
        ev->events |= events;
        epoll_ctl(m_epfd, EPOLL_CTL_MOD, ev->data.fd, ev);
    }
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
