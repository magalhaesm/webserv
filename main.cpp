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
        listener.subscribe(servers[i]);
    }

    listener.start();

}

