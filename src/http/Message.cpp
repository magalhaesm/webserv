#include <unistd.h>

#include "Message.hpp"

Message::Message()
    : body(0)
{
    ::clear(*this);
}

Message::~Message()
{
    this->clear();
}

void Message::clear()
{
    method = UNKNOWN;
    version.clear();
    path.clear();
    query.clear();
    headers.clear();
    if (body != 0)
    {
        close(body);
        body = 0;
    }
    unlink(bodyFilename.c_str());
    state = HEADERS;
    chunked = false;
    cLength = 0;
    written = 0;
    error = 0;
}

void Message::makeBody()
{
    char templateName[] = "request.XXXXXX";

    body = mkstemp(templateName);
    bodyFilename = templateName;
}

void clear(Message& msg)
{
    msg.method = UNKNOWN;
    msg.version.clear();
    msg.path.clear();
    msg.query.clear();
    msg.headers.clear();
    if (msg.body != 0)
    {
        close(msg.body);
    }
    msg.state = HEADERS;
    msg.chunked = false;
    msg.cLength = 0;
    msg.written = 0;
    msg.error = 0;
}
