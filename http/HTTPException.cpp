#include "HTTPConstants.hpp"
#include "HTTPException.hpp"

HTTPException::HTTPException(int status)
    : std::runtime_error(httpStatusCode(status))
    , _status(status)
{
}

int HTTPException::statusCode() const
{
    return _status;
}
