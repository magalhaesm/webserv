#ifndef EVENT_LISTENER_HPP
#define EVENT_LISTENER_HPP

#include <map>
#include <vector>
#include <sys/epoll.h>

#include "Server.hpp"
#include "Connection.hpp"
#include "Dispatcher.hpp"

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
     * @param target The server to subscribe.
     */
    void subscribe(Server* target);

    /**
     * @brief Starts the event listener.
     */
    void start();

private:
    /**
     * @brief Maximum number of events to handle.
     */
    static const int MAX_EVENTS = 10;

    int m_epollFd;                           /** File descriptor for epoll. */
    struct epoll_event m_events[MAX_EVENTS]; /** Array to store epoll events. */
    std::map<int, Server*> m_servers;        /** Map to store server instances. */
    Dispatcher m_dispatcher;                 /** Dispatcher for managing connections. */

    /**
     * @brief Waits for and handles events, creating new connections or
     * notifying events on existing ones.
     */
    void waitAndHandleEvents();

    /**
     * @brief Starts the subscribed servers.
     */
    void startServers();

    /**
     * @brief Watches a socket for events.
     * @param socket The socket to watch.
     */
    void watch(int socket);

    /**
     * @brief Finds the server associated with a socket for initiating a connection.
     * @param socket The socket for which to find the associated server.
     * @return The server associated with the socket, or NULL if not found.
     */
    Server* findReceiver(int socket);

    /**
     * @brief Accepts a connection from a client to the specified server.
     * @param server The server to accept the connection on.
     */
    void accept(Server* server);
};

#endif // !EVENT_LISTENER_HPP
