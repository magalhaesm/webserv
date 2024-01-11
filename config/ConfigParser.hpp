#ifndef ConfigParser_HPP
#define ConfigParser_HPP

#include "Libs.hpp"
#include "ConfigSpec.hpp"

class ConfigParser
{
    public:
        // Constructors
        ConfigParser(void);
        explicit ConfigParser(std::string const &FilePath);

        // Copy constructor
        ConfigParser(ConfigParser const &src);

        // Destructor
        ~ConfigParser(void);

        // Copy assignment operator
        ConfigParser &operator=(ConfigParser const &rhs);

        void handleConfigFile(char *filePath);
        bool checkBracketsMatch(void);
        bool validateServerBlock(void);
        void extractServerBlocks(void);     
        void parseServerBlocks(void);
        void parseDirectives(const std::string &serverBlock);
        void trim(std::string &string);
        void extractLocationBlocks(std::string &serverBlock);
        void parseLocationBlocks(void);
        void extractLocationPath(const std::string  &block);
        void parseDirectivesInLocation(const std::string &block);

        //getter:
        const std::vector<ConfigSpec>& getConfigSpecs() const;

        //debug:
        void printExtractedServerBlocks(void) const;
        void printParsedDirectives(void) const;
        void printLocationBlocks(void) const;
        void printParsedLocationBlocks(void) const;
        void printAllConfigSpecs(void) const;

    private:
        std::string _filePath;
        std::string _configFile;
        std::vector<std::string> _serverBlocks;
        std::map<std::string, std::vector<std::string> > _parsedDirectives;
        std::vector<std::string> _locationBlocks;  
        std::string _locationPath;
        std::map<std::string, std::string> _locationDirectives;    
        std::map<std::string, std::map<std::string, std::string> > _parsedLocationBlocks; 
        std::vector<ConfigSpec> _configSpecs;
};

#endif

/*

location blocks:

    location /website2 {
        autoindex off
        limit_except GET POST
        error_page 500 custom_500.html
    }

onde armazenar:

1 - std::map<std::string, std::map<std::string, std::vector<std::string>>>

   [ /website2 ] [ limit_except ] [ get ] [ post ] 

    - mais dificil par armazenar, mais facil para obter as informacoes

2 - std::map<std::string, std::map<std::string, std::string>>

    [ /website ] [ limit_except ] [ get post ]

    - mais facil de armazenar, mais dificil para obter as informacoes
    - teria que cortar a string " get post " para obter os dois valores
    - por outro lado so duas diretrizes tem multiplos valores no location: 
        - limit_except
        - error_page

*/