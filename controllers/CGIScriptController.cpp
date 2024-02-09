#include "CGIScriptController.hpp"

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

#include "HTTPResponse.hpp"


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

std::string CGIScriptController::handleRequest(const HTTPRequest& request) //recebe req cgi
{
    //  configurar as variáveis de ambiente para o CGI
    setCGIEnvironment(request);
    std::string cgiOutput;

    /*  funcao para ler o conteudo da msg http que vai para o cgi?
        std::string postData = request.getBody(); // Assumindo que getBody() retorna o corpo da requisição
        aqui colocamos hardcoded    */
    std::string postData = "length=10&special_chars=s&digits=s&uppercase=s";

    /*  se utilizasse um objeto response na executeScript para retornar o processamento do CGI
        HTTPResponse response; -> isto iria entrar como argumento da executeScript... */

    //  executa o sript em python
    cgiOutput = executeScript("./cgi/password_generator.py", postData);  

    //  constrói uma resposta HTTP hardcoded com a saída do script CGI
    std::string httpResponse = "HTTP/1.1 200 OK\r\n"
                               "Content-Type: text/html\r\n"
                               "Content-Length: " + std::to_string(cgiOutput.size()) + "\r\n"
                               "\r\n" +
                               cgiOutput;

    /*  Envia a resposta HTTP de volta ao cliente
        send(socket, httpResponse.c_str(), httpResponse.size(), 0); */

    return httpResponse;
}


void CGIScriptController::setCGIEnvironment(const HTTPRequest& request) //configurar variaveis de ambiente
{
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
}

std::string CGIScriptController::executeScript(const std::string& scriptPath, const std::string& postData) {
    int pipefd[2];      // pipe para capturar a saída do script CGI
    int inputPipe[2];   // pipe para passar os dados da requisição POST para o stdin do script CGI
    pid_t pid;
    std::string cgiOutput;

    // criação dos pipes
    if (pipe(pipefd) == -1 || pipe(inputPipe) == -1)
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
    {   // processo filho
        // fecha os lados não utilizados dos pipes
        close(pipefd[0]);
        close(inputPipe[1]);

        // redireciona stdout e stdin
        dup2(pipefd[1], STDOUT_FILENO);
        dup2(inputPipe[0], STDIN_FILENO);

        // fecha os descritores duplicados
        close(pipefd[1]);
        close(inputPipe[0]);

        // executa o script CGI
        execl("/usr/bin/python3", "python3", scriptPath.c_str(), (char *)NULL);
        
        // se execl falhar
        perror("execl");
        exit(EXIT_FAILURE);
    } 
    else 
    { 
        // processo pai
        // fecha os lados não utilizados dos pipes
        close(pipefd[1]);
        close(inputPipe[0]);

        // escreve os dados da requisição POST no stdin do script CGI
        if (write(inputPipe[1], postData.c_str(), postData.size()) == -1) 
        {
            perror("write to inputPipe");
        }
        close(inputPipe[1]); // fecha o lado de escrita após escrever os dados

        // lê a saída do script CGI
        char buffer[1024];
        ssize_t nbytes;
        while ((nbytes = read(pipefd[0], buffer, sizeof(buffer))) > 0)
        {
            cgiOutput.append(buffer, nbytes);
        }
        if (nbytes == -1) 
        {
            perror("read from pipefd");
        }
        close(pipefd[0]);

        // espera o processo filho terminar
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) 
        {
            std::cout << "Script CGI exited with status " << WEXITSTATUS(status) << std::endl;
        } 
        else if (WIFSIGNALED(status)) 
        {
            std::cout << "Script CGI killed by signal " << WTERMSIG(status) << std::endl;
        }
    }

    return cgiOutput;
}
