#ifndef EXEPARSER_H
#define EXEPARSER_H

#include <fstream>

namespace ExeParser
{
    void parse(char * path, std::ios::pos_type & contentStart, std::ios::pos_type & contentEnd);
};

#endif
