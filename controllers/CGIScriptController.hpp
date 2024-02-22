#ifndef CGIScriptController_HPP
#define CGISScriptController_HPP

#include <cstdlib>
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"


class CGIScriptController
{
    public:
        // Constructors
        CGIScriptController(void);

        // Copy constructor
        CGIScriptController(CGIScriptController const &src);

        // Destructor
        ~CGIScriptController(void);

        // Copy assignment operator
        CGIScriptController &operator=(CGIScriptController const &rhs);

        //methods:
        std::string handleRequest(const HTTPRequest& request); //recebe req cgi
        void setCGIEnvironment(const HTTPRequest& request); //configurar variaveis de ambiente
        std::string executeScript(const std::string& scriptPath, const std::string& postData); //executa o script CGI fazendo fork e o pipe

    private:

};

#endif

