#ifndef HTTP_REQUEST_MODEL_HPP
#define HTTP_REQUEST_MODEL_HPP

#include <string>

class HTTPRequestModel
{
public:
    HTTPRequestModel(std::string rawData); // TODO: Validar atributos
    std::string getMethod() const;
    std::string getPath() const;
    std::string getHeader() const;
    std::string getBody() const;
    std::string getHost() const;

private:
    std::string method; // GET, POST, DELETE
    std::string path;
    std::string header;
    std::string body;
    std::string host;
};

#endif // !HTTP_REQUEST_MODEL_HPP
