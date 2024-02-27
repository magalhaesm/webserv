#include "ARequestHandler.hpp"

ARequestHandler ::ARequestHandler(ARequestHandler* next)
    : _next(next)
{
}

ARequestHandler::~ARequestHandler()
{
}
