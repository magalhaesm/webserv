#include "CGIScriptController.hpp"

#include <iostream>
#include <cstdlib>
#include <unistd.h>


CGIScriptController::CGIScriptController(void)
{
    return;
}

CGIScriptController::CGIScriptController(CGIScriptController const &src)
{
    *this = src;
}

CGIScriptController::~CGIScriptController(void)
{
    return;
}

CGIScriptController &CGIScriptController::operator=(CGIScriptController const &rhs)
{
    // Check for self-assignment to avoid stack overflow
    if (this != &rhs) 
        *this = rhs; 
    return *this;
}


//methods:

void CGIScriptController::handleRequest(const HTTPRequest& request) //recebe req cgi
{
    // Configurar as variáveis de ambiente para o CGI
    std::cout << "inside handleRequest()" << std::endl;
    setCGIEnvironment(request);

    //executa o sript em python
    executeScript("./cgi/password_generator.py");
    
}


void CGIScriptController::setCGIEnvironment(const HTTPRequest& request) //configurar variaveis de ambiente
{
    std::cout << "inside setCGIEnvironment()" << std::endl;

    /* setenv("REQUEST_METHOD", request.method().c_str(), 1); */
    std::string request_method = "POST";
    setenv("REQUEST_METHOD", request_method.c_str(), 1);

    // Para POST, você pode querer definir CONTENT_LENGTH e CONTENT_TYPE
    if (request.method() == "POST")
    {
        /* std::string contentLength = request.get("Content-Length");
           std::string contentType = request.get("Content-Type"); */
        
        std::string content_length = "46";
        std::string content_type = "application/x-www-form-urlencoded";

        /* setenv("CONTENT_LENGTH", contentLength.c_str(), 1);
           setenv("CONTENT_TYPE", contentType.c_str(), 1); */

        setenv("CONTENT_LENGTH", content_length.c_str(), 1);
        setenv("CONTENT_TYPE", content_type.c_str(), 1);
    }

    //verificando se as variaveis de ambiente foram configuradas
    extern char **environ;    
    for (char **env = environ; *env != 0; env++)
    {
        char *thisEnv = *env;
        std::cout << thisEnv << std::endl; // Imprime a variável de ambiente atual
    }
}


void CGIScriptController::executeScript(const std::string& scriptPath) //executa o script CGI fazendo fork e o pipe
{
    std::cout << "executeScript" << std::endl;
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    { // Processo filho
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        // Executar o script Python
        execl("/usr/bin/python3", "python", scriptPath.c_str(), (char *)NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    } 
    else
    { // Processo pai
        char buffer[128];
        int nbytes;
        close(pipefd[1]);

        std::cout << "executeScript: antes do while" << std::endl;

        while ((nbytes = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            std::cout.write(buffer, nbytes);
        }
 
        std::cout << "executeScript: apos  while" << std::endl;

        close(pipefd[0]);

        std::cout << "executeScript: apos pipe" << std::endl;

    }
}
