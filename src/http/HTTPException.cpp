#include "HTTPConstants.hpp"
#include "HTTPException.hpp"

HTTPException::HTTPException(int status)
    : std::runtime_error(statusText(status))
    , _status(status)
{
}

int HTTPException::statusCode() const
{
    return _status;
}
