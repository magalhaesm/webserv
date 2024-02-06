#include <cerrno>
#include <string>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <stdexcept>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "ConfigParser.hpp"
#include "Server.hpp"
#include "Connection.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "CGIScriptController.hpp"

/* CGI */
#include <fstream>
#include <sstream>


const int BACKLOG = 10;

static void fatalError(const std::string& errMsg);

Server::Server(ConfigSpec& cfg)
    : m_port(cfg.getPort())
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

int Server::accept()
{
    return ::accept(m_socket, NULL, NULL);
}

bool Server::read(Connection* conn)
{
    HTTPRequest* request = conn->request();
    CGIScriptController cgi;

    std::cout << "Funcao Server::read()" << std::endl;
    /* CGI */

    std::string request_url = request->URL();
    if (request_url.find("/cgi/") != std::string::npos)
    {
        std::cout << "Server::read() - IF" << std::endl;
        std::cout << "URL: " << request->URL() << std::endl;

        std::cout << "Request Method: " << request->method() << std::endl;

        if (request->method() == "POST")
            cgi.handleRequest(*request);
        else
            return serveCGIPage(conn);
    }
    return true;
}

bool Server::write(Connection* conn)
{
    /* CGI */
    HTTPRequest* request = conn->request();
 
    HTTPResponse* response = conn->response();

    /* CGI */
    // Verifica se a URL da requisição corresponde à página cgi.html
    if (request->URL() == "/cgi/cgi.html") {
        // Tenta abrir e ler o arquivo cgi.html
        std::ifstream file("cgi/cgi.html");
        if (file.is_open()) {
            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string content = buffer.str();

            // Configura a resposta com o conteúdo do arquivo cgi.html
            response->setStatus(200);
            response->set("Content-Type", "text/html");
            response->setBody(content);

            // Converte a resposta HTTP em uma string para envio
            std::string httpResponse = response->toString();

            // Envia a resposta HTTP para o cliente
            conn->write(httpResponse);

            return true; // Retorna true para indicar que a resposta foi enviada com sucesso
        }
    }

    // Se não for uma requisição para cgi.html, use a resposta hardcoded padrão
    const std::string html = "<html><body><h1>Hello, World! HELLO! </h1></body></html>";
    response->setStatus(200);
    response->set("Content-Type", "text/html");
    response->setBody(html);
    return true;

    
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

//    std::cout << "createSocket: m_port: " << m_port <<std::endl;
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

void fatalError(const std::string& errMsg)
{
    std::cerr << errMsg << ": " << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
}


// Teste CGI

bool Server::serveCGIPage(Connection* conn)
{
    std::cout << "serveCGIPage" << std::endl;

    std::string path = conn->request()->URL();

    if (path == "/cgi/cgi.html") {
        std::ifstream file("cgi/cgi.html");
        std::stringstream buffer;

        buffer << file.rdbuf();
        std::string content = buffer.str();

        HTTPResponse* response = conn->response();
        response->setStatus(200);
        response->set("Content-Type", "text/html");
        response->setBody(content);

        return true; // Indica que a página foi servida
    }

    return false; // Indica que a requisição não corresponde a uma página CGI
}