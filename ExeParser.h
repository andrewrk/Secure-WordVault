#ifndef EXEPARSER_H
#define EXEPARSER_H

#include <QFile>

namespace ExeParser
{
    void parse(QFile & openFile, qint64 & contentStart, qint64 & contentEnd);
};

#endif
