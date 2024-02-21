#ifndef BODY_HPP
#define BODY_HPP

#include "definitions.hpp"

class Body
{
public:
    Body(BodyType type, BodyContent content);
    Body(const Body& rhs);
    Body& operator=(const Body& rhs);
    ~Body();

    BodyType getType() const;
    const std::string& get(const std::string& key);

private:
    BodyType m_type;
    BodyContent m_content;

    std::string m_empty;
};

#endif // !BODY_HPP
