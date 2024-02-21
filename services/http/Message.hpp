#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <map>
#include <string>

class Body;
class ABodyParser;
typedef std::map<std::string, std::string> Headers;
typedef std::map<std::string, std::string> BodyContent;

enum Method
{
    GET,
    POST,
    DELETE,
    UNKNOWN
};

enum ParsingState
{
    HEADERS,
    BODY,
    BODY_TYPE,
    BODY_CONTENT,
    FINISH
};

enum BodyType
{
    URLEncoded,
    FormData,
    Chunked
};

struct Message
{
    Method method;
    std::string version;
    std::string path;
    std::string query;
    Headers headers;
    Body* body;
    ABodyParser* parser;
    ParsingState state;

    Message();
};

void clear(Message& msg);

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

#endif // !MESSAGE_HPP
