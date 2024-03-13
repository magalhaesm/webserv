#ifndef INDEX_HANDLER_HPP
#define INDEX_HANDLER_HPP

#include "ARequestHandler.hpp"

class IndexContentHandler : public ARequestHandler
{
public:
    IndexContentHandler();
    void handle(Request& req, Response& res, const ConfigSpec& cfg);
};

#endif // !INDEX_HANDLER_HPP
