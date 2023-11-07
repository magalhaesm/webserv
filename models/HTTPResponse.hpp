#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <string>

// - Representa respostas HTTP.
// - Encapsula informações, como status code e corpo.
class HTTPResponse
{
public:
    HTTPResponse(std::string rawData); // TODO: Validar atributos

    void setStatus(int status);
    int getStatusCode() const;

    std::string toString() const;

private:
    int statusCode;
    std::string version;
    std::string contentType;
    std::string body;
};

#endif // !HTTP_RESPONSE_HPP
