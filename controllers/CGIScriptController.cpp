#include "CGIScriptController.hpp"


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

void CGIScriptController::handleRequest(const HTTPRequest& request, HTTPResponse& response) //recebe req cgi
{
    // Configurar as variáveis de ambiente para o CGI
    setCGIEnvironment(request);

    // Aqui você pode invocar o script CGI ou realizar outras operações
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


void CGIScriptController::executeScript(const std::string& scriptPath) //executa o script CGI fazendo fork e o pipe
{
    return;
}
