#ifndef FORMDATA_PARSER_HPP
#define FORMDATA_PARSER_HPP

#include <fstream>

#include "ABodyParser.hpp"

class FormDataParser : public ABodyParser
{
public:
    FormDataParser(std::string& raw, Message& msg, size_t maxSize);
    Body* createBody();

private:
    std::string _boundaryStart;
    std::string _boundaryEnd;
    static const size_t BOUNDARY_LENGTH;
    std::ofstream _file;

    void setBoundary();
    void parseRawBody();
};

#endif // !FORMDATA_PARSER_HPP
