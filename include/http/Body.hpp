#ifndef BODY_HPP
#define BODY_HPP

#include "HTTPConstants.hpp"

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
    BodyType _type;
    BodyContent _content;

    std::string _empty;
};

#endif // !BODY_HPP
