#include <unistd.h>

#include "Message.hpp"

Message::Message()
    : body(0)
{
    this->clear();
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
    clength = 0;
    written = 0;
    error = 0;
}

void Message::makeBody()
{
    char templateName[] = "request.XXXXXX";

    body = mkstemp(templateName);
    bodyFilename = templateName;
}
