#include <cerrno>
#include <cstring>

#include "InternalErrorException.hpp"

InternalErrorException::InternalErrorException(const std::string& message)
    : std::invalid_argument(message + ": " + strerror(errno))
    , _errnum(errno)
{
}

int InternalErrorException::errnum() const
{
    return _errnum;
}
