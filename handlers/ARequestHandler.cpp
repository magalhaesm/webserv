#include "ARequestHandler.hpp"

ARequestHandler::ARequestHandler(const ConfigSpec& cfg)
    : _cfg(cfg)
    , _next(NULL)
{
}

ARequestHandler::~ARequestHandler()
{
}

void ARequestHandler::setNext(ARequestHandler* handler)
{
    _next = handler;
}

void ARequestHandler::setContext(const ConfigSpec&)
{
    // _cfg = cfg;
}
