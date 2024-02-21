#include "Body.hpp"

Body::Body(BodyType type, BodyContent content)
    : m_type(type)
    , m_content(content)
{
}

Body::Body(const Body& rhs)
    : m_type(rhs.m_type)
    , m_content(rhs.m_content)
{
}

Body& Body::operator=(const Body& rhs)
{
    if (&rhs != this)
    {
        m_type = rhs.m_type;
        m_content = rhs.m_content;
    }
    return *this;
}

Body::~Body()
{
}

BodyType Body::getType() const
{
    return m_type;
}

const std::string& Body::get(const std::string& key)
{
    BodyContent::iterator it = m_content.find(key);
    if (it != m_content.end())
    {
        return it->second;
    }
    return m_empty;
}
