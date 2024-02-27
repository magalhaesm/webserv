#ifndef EVENT_LISTENER_HPP
#define EVENT_LISTENER_HPP

#include <map>
#include <sys/epoll.h>

#include "Server.hpp"
#include "Connection.hpp"

/**
 * @class EventListener
 * @brief Manages the single `epoll()` for all I/O operations in the server.
 *
 * The EventListener class performs simultaneous read and write checks.
 * When an event occurs, it is the responsibility of the class to create a new
 * connection between the client and server. If the connection already exists,
 * the class notifies the server of a client action.
 */
class EventListener
{
public:
    /**
     * @brief Constructor for EventListener.
     */
    EventListener();

    /**
     * @brief Destructor for EventListener.
     */
    ~EventListener();

    /**
     * @brief Subscribes a server for event notifications.
     * @param server The server to subscribe.
     */
    void subscribe(Server* server);

    /**
     * @brief Starts the event listener.
     */
    void start();

    void close(Connection* conn);

private:
    /**
     * @brief Maximum number of events to handle.
     */
    static const int MAX_EVENTS = 10;

    int m_epfd;                              /** File descriptor for epoll. */
    struct epoll_event m_events[MAX_EVENTS]; /** Array to store epoll events. */
    std::map<int, Server*> m_servers;        /** Map to store server instances. */
    std::map<int, Connection*> m_active;     /** Map to store active connections. */

    /**
     * @brief Starts the subscribed servers.
     */
    void startServers();

    bool establishConnection(int socket);
    void handleEvent(struct epoll_event* event);
    void update(bool change, struct epoll_event* event, uint32_t events);
    void checkTimeout(std::time_t threshold);
};

#endif // !EVENT_LISTENER_HPP
