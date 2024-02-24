#include <set>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <cstring>
#include <iostream>

#include "strings.hpp"
#include "ConfigParser.hpp"

ConfigParser::ConfigParser(const std::string& filename)
    : _filename(filename)
    , _config(_filename.c_str())
    , _state(OutsideServerBlock)
    , _openBlocks(0)
    , _currentDirectives(NULL)
    , _lineNumber(0)
{
    if (!_config)
    {
        throw std::runtime_error(filename + ": " + strerror(errno));
    }

    initializeValidationMap();
    this->parse();
}

ConfigParser::~ConfigParser()
{
}

const ConfigSpecList& ConfigParser::getSpecs()
{
    if (_specs.empty())
    {
        for (size_t idx = 0; idx < _directives.size(); ++idx)
        {
            _specs.push_back(ConfigSpec(_directives[idx]));
        }
    }
    return _specs;
}

void ConfigParser::parse()
{
    std::string line;
    while (std::getline(_config, line))
    {
        _lineNumber++;
        ft::trim(line);
        if (isCommentOrEmpty(line))
        {
            continue;
        }

        Strings tokens = ft::strip(ft::split(line, " "));
        switch (_state)
        {
        case OutsideServerBlock:
        {
            findNewServerBlock(tokens);
            break;
        }
        case InsideServerBlock:
        {
            parseServerBlock(tokens);
            break;
        }
        case InsideLocationBlock:
        {
            parseLocationBlock(tokens);
            break;
        }
        }
    }
    checkOpenBlocks();
}

inline bool ConfigParser::isCommentOrEmpty(const std::string& line)
{
    return line[0] == '#' || line.empty();
}

inline void ConfigParser::findNewServerBlock(Strings& tokens)
{
    if (tokens[0] == "server")
    {
        validateServer(tokens);
        enterServerContext();
        _state = InsideServerBlock;
        _openBlocks++;
        return;
    }
    throw ParseException(fmtError("expecting 'server', got '" + tokens[0] + "'"));
}

inline void ConfigParser::parseServerBlock(const Strings& tokens)
{
    if (_keywords.count(tokens[0]))
    {
        ValidationMethod assign = _keywords.at(tokens[0]);
        (this->*assign)(tokens, _currentDirectives);
        return;
    }
    if (tokens[0] == "}")
    {
        _state = OutsideServerBlock;
        _openBlocks--;
        return;
    }
    if (tokens[0] == "location")
    {
        validateLocation(tokens);
        enterLocationContext(tokens[1]);
        _state = InsideLocationBlock;
        _openBlocks++;
        return;
    }
    throw ParseException(fmtError("unknown directive '" + tokens[0] + "'"));
}

inline void ConfigParser::parseLocationBlock(const Strings& tokens)
{
    if (tokens[0] == "listen" || tokens[0] == "server_name")
    {
        throw ParseException(fmtError("'" + tokens[0] + "' directive is not allowed here"));
    }
    if (_keywords.count(tokens[0]))
    {
        ValidationMethod assign = _keywords.at(tokens[0]);
        (this->*assign)(tokens, _currentDirectives);
        return;
    }
    if (tokens[0] == "}")
    {
        exitContext();
        _state = InsideServerBlock;
        _openBlocks--;
        return;
    }
    throw ParseException(fmtError("unknown directive '" + tokens[0] + "'"));
}

inline void ConfigParser::checkOpenBlocks()
{
    if (_openBlocks == 0)
    {
        return;
    }
    throw ParseException(fmtError("unexpected end of file, expecting '}'"));
}

void ConfigParser::initializeValidationMap()
{
    _keywords["listen"] = &ConfigParser::validateListen;
    _keywords["server_name"] = &ConfigParser::validateServerName;
    _keywords["index"] = &ConfigParser::validateIndex;
    _keywords["root"] = &ConfigParser::validateRoot;
    _keywords["autoindex"] = &ConfigParser::validateAutoindex;
    _keywords["error_page"] = &ConfigParser::validateErrorPage;
    _keywords["cgi"] = &ConfigParser::validateCGI;
    _keywords["redirect"] = &ConfigParser::validateRedirect;
    _keywords["limit_except"] = &ConfigParser::validateMethods;
    _keywords["client_body_size"] = &ConfigParser::validateClientBodySize;
}

std::string ConfigParser::fmtError(const std::string& message)
{
    std::stringstream ss;
    ss << message << " in " << _filename << ":" << _lineNumber;
    return ss.str();
}

inline void ConfigParser::validateServer(const Strings& tokens)
{
    if (tokens.size() != 2 || tokens[1] != "{")
    {
        throw ParseException(fmtError("directive 'server' has no opening '{'"));
    }
}

inline void ConfigParser::validateLocation(const Strings& tokens)
{
    checkArgCount(tokens, tokens[1] == "{");

    if (tokens.size() != 3)
    {
        throw ParseException(fmtError("directive 'location' has no opening '{'"));
    }
}

void ConfigParser::validateListen(const Strings& tokens, Directives* directive)
{
    checkArgCount(tokens, tokens.size() != 2);

    int port = std::atoi(tokens[1].c_str());
    if (port < 3 || port > 65535)
    {
        throw ParseException(fmtError("invalid port number '" + tokens[1] + "'"));
    }
    directive->listen = port;
}

void ConfigParser::validateServerName(const Strings& tokens, Directives* directive)
{
    checkArgCount(tokens, tokens.size() != 2);
    directive->server_name = tokens[1];
}

void ConfigParser::validateIndex(const Strings& tokens, Directives* directive)
{
    checkArgCount(tokens, tokens.size() != 2);
    directive->index = tokens[1];
}

void ConfigParser::validateRoot(const Strings& tokens, Directives* directive)
{
    checkArgCount(tokens, tokens.size() != 2);
    directive->root = tokens[1];
}

void ConfigParser::validateAutoindex(const Strings& tokens, Directives* directive)
{
    checkArgCount(tokens, tokens.size() != 2);
    if (tokens[1] != "on" && tokens[1] != "off")
    {
        throw ParseException(fmtError(
            "invalid value '" + tokens[1]
            + "' in 'autoindex' directive, it must be 'on' or 'off'"));
    }
    directive->autoindex = (tokens[1] == "on") ? true : false;
}

void ConfigParser::validateErrorPage(const Strings& tokens, Directives* directive)
{
    checkArgCount(tokens, tokens.size() < 3);
    std::string page = tokens[tokens.size() - 1];
    for (size_t idx = 1; idx < tokens.size() - 1; ++idx)
    {
        int error = std::atoi(tokens[idx].c_str());
        if (error < 300 || error > 599)
        {
            throw ParseException(
                fmtError("value '" + tokens[idx] + "' must be between 300 and 599"));
        }
        directive->error_page[error] = page;
    }
}

void ConfigParser::validateCGI(const Strings& tokens, Directives* directive)
{
    checkArgCount(tokens, tokens.size() != 2);
    directive->cgi = tokens[1];
}

void ConfigParser::validateRedirect(const Strings& tokens, Directives* directive)
{
    checkArgCount(tokens, tokens.size() != 3);
    int statusCode = std::atoi(tokens[1].c_str());
    directive->redirect = std::make_pair(statusCode, tokens[2]);
}

void ConfigParser::validateMethods(const Strings& tokens, Directives* directive)
{
    static std::set<std::string> methods;
    methods.insert("get");
    methods.insert("post");
    methods.insert("delete");

    for (size_t idx = 1; idx < tokens.size(); ++idx)
    {
        std::string method = ft::toLower(tokens[idx]);
        if (!methods.count(method))
        {
            throw ParseException(fmtError("invalid method '" + tokens[idx] + "'"));
        }
        directive->limit_except.insert(method);
    }
}

void ConfigParser::validateClientBodySize(const Strings& tokens, Directives* directive)
{
    checkArgCount(tokens, tokens.size() != 2);
    directive->client_max_body_size = std::atoi(tokens[1].c_str()) << 20;
}

inline void ConfigParser::enterServerContext()
{
    _directives.push_back(Directives());
    _contextStack.push(&_directives.back());
    _currentDirectives = &_directives.back();
}

inline void ConfigParser::enterLocationContext(const std::string& location)
{
    _currentDirectives->locations[location] = Directives();
    _contextStack.push(&_currentDirectives->locations[location]);
    _currentDirectives = _contextStack.top();
}

inline void ConfigParser::exitContext()
{
    _contextStack.pop();
    _currentDirectives = _contextStack.top();
}

void ConfigParser::checkArgCount(const Strings& tokens, bool badCondition)
{
    if (badCondition)
    {
        std::string message = "invalid number of arguments for directive '";
        throw ParseException(fmtError(message + tokens[0] + "'"));
    }
}

ConfigParser::ParseException::ParseException(const std::string& err)
    : std::invalid_argument(err)
{
}
