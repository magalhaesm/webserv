#ifndef ACCESS_CONTROL_HANDLER_HPP
#define ACCESS_CONTROL_HANDLER_HPP

#include "ARequestHandler.hpp"

class AccessControlHandler : public ARequestHandler
{
public:
    AccessControlHandler();
    void handle(HTTPRequest& req, HTTPResponse& res, const ConfigSpec& cfg);
};

#endif // !ACCESS_CONTROL_HANDLER_HPP
