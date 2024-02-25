#ifndef FORMDATA_PARSER_HPP
#define FORMDATA_PARSER_HPP

#include <fstream>

#include "ABodyParser.hpp"

class FormDataParser : public ABodyParser
{
public:
    FormDataParser(std::string& raw, Message& msg);
    Body* createBody();

private:
    std::string m_boundaryStart;
    std::string m_boundaryEnd;
    static const size_t BOUNDARY_LENGTH;
    std::ofstream file;

    void parseRawBody();
};

#endif // !FORMDATA_PARSER_HPP
