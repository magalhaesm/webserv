#include "Body.hpp"

Body::Body(BodyType type, BodyContent content)
    : _type(type)
    , _content(content)
{
}

Body::Body(const Body& rhs)
    : _type(rhs._type)
    , _content(rhs._content)
{
}

Body& Body::operator=(const Body& rhs)
{
    if (&rhs != this)
    {
        _type = rhs._type;
        _content = rhs._content;
    }
    return *this;
}

Body::~Body()
{
}

BodyType Body::getType() const
{
    return _type;
}

const std::string& Body::get(const std::string& key)
{
    BodyContent::iterator it = _content.find(key);
    if (it != _content.end())
    {
        return it->second;
    }
    return _empty;
}
