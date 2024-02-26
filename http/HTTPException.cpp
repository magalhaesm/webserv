#include "HTTPException.hpp"

HTTPException::HTTPException(int status)
    : std::runtime_error("TODO: implement http error message")
    , _status(status)
{
}

int HTTPException::statusCode()
{
    return _status;
}
