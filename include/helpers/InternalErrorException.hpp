#ifndef INTERNAL_ERROR_EXCEPTION_HPP
#define INTERNAL_ERROR_EXCEPTION_HPP

#include <stdexcept>

class InternalErrorException : public std::invalid_argument
{
public:
    InternalErrorException(const std::string& message);
    int errnum() const;

private:
    int _errnum;
};

#endif // !INTERNAL_ERROR_EXCEPTION_HPP
