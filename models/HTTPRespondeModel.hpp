#ifndef HTTP_RESPONSE_MODEL_HPP
#define HTTP_RESPONSE_MODEL_HPP

#include <string>

class HTTPResponseModel
{
public:
    HTTPResponseModel(std::string rawData); // TODO: Validar atributos

    void setStatus(int status);
    void setContentType();

    int getStatusCode() const;
    std::string getContentType() const;

    std::string toString() const;

private:
    int statusCode;
    std::string contentType;
    std::string body;
};

#endif // !HTTP_RESPONSE_MODEL_HPP
