#include <iostream>
#include "Server.hpp"
#include "EventListener.hpp"

#include "ConfigParser.hpp" 

#include <csignal>


int main(int argc, char **argv)
{
    ConfigParser configParser;
    
    if (argc != 2)
    {
        if (argc < 2)
            std::cerr << "Error: missing ConfigParseruration file" << std::endl;
        if (argc > 2)
            std::cerr << "Error: too many arguments" << std::endl;
        return (1);
    }

    configParser.handleConfigFile(argv[1]);

    std::vector<ConfigSpec> specs;
    specs = configParser.getConfigSpecs();

    std::vector<Server*> servers(specs.size());

    EventListener listener;

    for(size_t i = 0; i < specs.size(); i++)
    {
        servers[i] = new Server(specs[i]);
        

        
        /* ---- printing getters ---- */
        
        std::cout << "\nGETTERS SERVER " << i << ":" << std::endl;

        int ports = specs[i].getPort();
        std::cout << "  Ports: ";
        std::cout << ports << std::endl;

        std::string serverNames = specs[i].getServerName();
        std::cout << "  Names: ";
        std::cout << serverNames << std::endl;

        std::cout << std::endl;

        std::string index = specs[i].getIndex();
        std::cout << "  Index: ";
        std::cout << index << std::endl;

        std::string root = specs[i].getRoot();
        std::cout << "  Root: ";
        std::cout << root << std::endl;

        std::string autoindex = specs[i].getAutoindex();
        std::cout << "  Autoindex: ";
        std::cout << autoindex << std::endl;

        std::vector<std::string> errorPage = specs[i].getErrorPage();
        std::cout << "  Error Page: ";
        for (size_t j = 0; j < errorPage.size(); ++j) {
            std::cout << errorPage[j] << " ";
        }
        std::cout << std::endl;

        std::string cgi = specs[i].getCgi();
        std::cout << "  Cgi: ";
        std::cout << cgi << std::endl;

        std::cout << std::endl;

        std::string redirect = specs[i].getRedirect();
        std::cout << "  redirect: ";
        std::cout << redirect << std::endl;
        std::cout << std::endl;



        
        /* -----------------------------*/
        

        listener.subscribe(servers[i]);
    }

    listener.start();

}

