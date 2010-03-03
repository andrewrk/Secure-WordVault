#ifndef EXEPARSER_H
#define EXEPARSER_H

#include <QFile>

class ExeParser
{
public:
    // get the document out of an EXE. size of 0 means no document
    static QByteArray read(QString exeFile);

    // (over)write a document into an EXE.
    static void write(QString exeFile, QByteArray document);

    // copy an exe file, leaving the document behind
    static void copyOnlyExe(QString exeFileSource, QString exeFileDest);

private:
    static void parse(QFile & openFile, qint64 & contentStart, qint64 & contentEnd);

};

#endif
