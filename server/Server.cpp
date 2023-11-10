#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <iostream> // TODO: remover

#include "Server.hpp"
#include "Connection.hpp"

void fatalError(const std::string& errMsg)
{
    std::cerr << errMsg << ": " << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
}

const int BACKLOG = 10;

Server::Server(const ConfigSpec& cfg)
    : m_name(cfg.getServerName())
    , m_host(cfg.getHostName())
    , m_port(cfg.getPort())
{
    m_socket = createSocket();
}

Server::~Server()
{
    close(m_socket);
}

void Server::listen()
{
    if (::listen(m_socket, BACKLOG) == -1)
    {
        close(m_socket);
        fatalError("Error while listening");
    }
}

#include <stdio.h>
// TODO: ler e escrever no socket
void Server::handleIncomingData(Connection* conn)
{
    // conn.request.status;
    // conn.response.status;

    int socket = conn->getSocket();
    char data[1024];
    ssize_t nBytes = read(socket, data, sizeof(data));
    if (nBytes <= 0) // Navegador fechou a conexão
    {
        printf("Conexão fechada: %d\n", socket);
        conn->close();
    }
    else
    {
        const char* response = "HTTP/1.1 200 OK\r\n"
                               "Content-Type: text/html\r\n"
                               "\r\n"
                               "<html><body><h1>Hello, World!</h1></body></html>";

        ssize_t response_size = strlen(response);
        ssize_t bytes_sent = write(socket, response, response_size);

        if (bytes_sent < 0)
        {
            perror("Erro ao enviar resposta");
        }
        else
        {
            printf("Resposta enviada com sucesso: %s\n", m_name.c_str());
        }

        conn->close();
    }
}

int Server::getSocket() const
{
    return m_socket;
}

int Server::createSocket()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        fatalError("Error creating socket");
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port);
    addr.sin_addr.s_addr = INADDR_ANY;

    int yes = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
        close(fd);
        fatalError("Error during socket initialization");
    }

    return fd;
}
