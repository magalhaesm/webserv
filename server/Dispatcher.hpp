#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include <map>
#include <sys/epoll.h>

#include "Server.hpp"
#include "Connection.hpp"

/**
 * @class Dispatcher
 * @brief Responsible for opening, closing, and notifying events on connections.
 */
class Dispatcher
{
public:
    /**
     * @brief Constructor for Dispatcher.
     */
    Dispatcher();

    /**
     * @brief Destructor for Dispatcher.
     */
    ~Dispatcher();

    /**
     * @brief Connects a server, returning the associated Connection.
     * @param server The server to connect.
     * @return The Connection associated with the server.
     */
    Connection* connect(Server* server);

    /**
     * @brief Closes the specified connection.
     * @param connection The connection to close.
     */
    void close(Connection* connection);

    /**
     * @brief Notifies an event using the provided epoll_event structure.
     * @param event The epoll_event structure representing the event.
     */
    void notify(struct epoll_event* event);

private:
    std::map<int, Connection*> active; /** Map to store active connections. */

    /**
     * @brief Gets the Connection associated with the specified socket.
     * @param socket The socket for which to find the associated Connection.
     * @return The Connection associated with the socket, or NULL if not found.
     */
    Connection* get(int socket);
};

#endif // !DISPATCHER_HPP
