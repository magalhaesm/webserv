#ifndef DIRECTIVES_HPP
#define DIRECTIVES_HPP

#include <map>
#include <set>
#include <string>

struct Directives;
typedef std::map<std::string, Directives> Locations;

struct Redirect
{
    int code;
    std::string url;
};

struct Directives
{
    int listen;
    std::string server_name;
    std::string index;
    std::string root;
    std::string cgi;
    bool autoindex;
    std::map<int, std::string> error_page;
    Redirect redirect;
    std::set<std::string> limit_except;
    int client_max_body_size;
    std::string upload_dir;
    Locations locations;
};

#endif // !DIRECTIVES_HPP
