#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <map>
#include <stack>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

#include "strings.hpp"
#include "ConfigSpec.hpp"
#include "Directives.hpp"

using ft::Strings;

class ConfigParser;
typedef void (ConfigParser::*ValidationMethod)(const Strings&, Directives*);
typedef std::map<std::string, ValidationMethod> ValidationMap;
typedef std::vector<ConfigSpec> ConfigSpecList;

class ConfigParser
{
public:
    ConfigParser(const std::string& filename);
    ~ConfigParser();

    const ConfigSpecList& getSpecs();

private:
    enum State
    {
        OutsideServerBlock,
        InsideServerBlock,
        InsideLocationBlock
    };

    std::string _filename;
    std::ifstream _config;
    ValidationMap _keywords;
    std::vector<Directives> _directives;
    State _state;
    int _openBlocks;
    Directives* _currentDirectives;
    int _lineNumber;
    std::stack<Directives*> _contextStack;
    ConfigSpecList _specs;

    void parse();
    bool isCommentOrEmpty(const std::string& line);
    void findNewServerBlock(Strings& tokens);
    void parseServerBlock(const Strings& tokens);
    void parseLocationBlock(const Strings& tokens);
    void checkOpenBlocks();
    void initializeValidationMap();
    std::string fmtError(const std::string& message);

    void validateServer(const Strings& tokens);
    void validateLocation(const Strings& tokens);
    void validateListen(const Strings& tokens, Directives* directive);
    void validateServerName(const Strings& tokens, Directives* directive);
    void validateIndex(const Strings& tokens, Directives* directive);
    void validateRoot(const Strings& tokens, Directives* directive);
    void validateAutoindex(const Strings& tokens, Directives* directive);
    void validateErrorPage(const Strings& tokens, Directives* directive);
    void validateCGI(const Strings& tokens, Directives* directive);
    void validateRedirect(const Strings& tokens, Directives* directive);
    void validateMethods(const Strings& tokens, Directives* directive);
    void validateClientBodySize(const Strings& tokens, Directives* directive);

    void enterServerContext();
    void enterLocationContext(const std::string& location);
    void exitContext();
    void checkArgCount(const Strings& tokens, bool badCondition);

    class ParseException : public std::invalid_argument
    {
    public:
        ParseException(const std::string& err);
    };
};

#endif // !CONFIG_PARSER_HPP
