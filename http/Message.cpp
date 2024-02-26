#include "Message.hpp"
#include "ABodyParser.hpp"

Message::Message()
    : body(NULL)
    , parser(NULL)
{
    clear(*this);
}

void clear(Message& msg)
{
    msg.method = UNKNOWN;
    msg.version.clear();
    msg.path.clear();
    msg.query.clear();
    msg.headers.clear();
    msg.state = HEADERS;
    msg.bodySize = 0;
    msg.error = 0;

    if (msg.body != NULL)
    {
        delete msg.body;
        msg.body = NULL;
    }
    if (msg.parser != NULL)
    {
        delete msg.parser;
        msg.parser = NULL;
    }
}
