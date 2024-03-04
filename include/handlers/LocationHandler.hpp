#ifndef LOCATION_HANDLER_HPP
#define LOCATION_HANDLER_HPP

#include "ARequestHandler.hpp"

class LocationHandler : public ARequestHandler
{
public:
    LocationHandler();
    void handle(Request& req, Response& res, const ConfigSpec& cfg);
};

#endif // !LOCATION_HANDLER_HPP
