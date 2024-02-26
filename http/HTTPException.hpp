#ifndef HTTP_EXCEPTION_HPP
#define HTTP_EXCEPTION_HPP

#include <stdexcept>

class HTTPException : public std::runtime_error
{
public:
    HTTPException(int status);
    int statusCode();

private:
    int _status;
};

#endif // !HTTP_EXCEPTION_HPP
