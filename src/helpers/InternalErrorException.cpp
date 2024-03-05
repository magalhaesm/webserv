#include <cerrno>
#include <cstring>

#include "InternalErrorException.hpp"

InternalErrorException::InternalErrorException(const std::string& message)
    : std::runtime_error(message + ": " + strerror(errno))
    , _errnum(errno)
{
}

int InternalErrorException::errnum() const
{
    return _errnum;
}
