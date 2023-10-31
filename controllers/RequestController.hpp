#ifndef REQUEST_CONTROLLER_HPP
#define REQUEST_CONTROLLER_HPP

#include "HTTPRequestModel.hpp"
#include "HTTPResponseModel.hpp"
#include "CGIScriptController.hpp"

class RequestController
{
public:
    RequestController()
    {
    }

    // Método para lidar com uma solicitação HTTP
    void handleRequest(HTTPRequestModel& request, HTTPResponseModel& response)
    {
        // Verifica o método HTTP (GET, POST, DELETE)
        std::string method = request.getMethod();

        // Roteamento com base no método
        if (method == "GET")
        {
            handleGetRequest(request, response);
        }
        else if (method == "POST")
        {
            handlePostRequest(request, response);
        }
        else if (method == "DELETE")
        {
            handleDeleteRequest(request, response);
        }
        else
        {
            // Método não suportado
            response.setStatus(405, "Method Not Allowed");
            response.setBody("Method not allowed");
        }
    }

private:
    // Manipulador para solicitações GET
    void handleGetRequest(const HTTPRequestModel& request, HTTPResponseModel& response)
    {
        // Lógica para manipular solicitações GET
        // Pode incluir leitura de arquivos estáticos, geração dinâmica de conteúdo, etc.
    }

    // Manipulador para solicitações POST
    void handlePostRequest(const HTTPRequestModel& request, HTTPResponseModel& response)
    {
        // Lógica para manipular solicitações POST
    }

    // Manipulador para solicitações DELETE
    void handleDeleteRequest(const HTTPRequestModel& request, HTTPResponseModel& response)
    {
        // Lógica para manipular solicitações DELETE
    }
};

#endif
