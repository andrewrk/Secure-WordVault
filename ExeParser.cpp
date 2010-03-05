#include "ExeParser.h"

#include <QApplication>
#include <cassert>

// generated from http://www.guidgenerator.com/online-guid-generator.aspx
const unsigned char guid[] = {0x7c, 0x7a, 0x64, 0x0e, 0x9b, 0x4f, 0x42, 0x58, 0xa8, 0xb4, 0xd8, 0x4b, 0xd5, 0x8a, 0x05, 0x14};

void ExeParser::parse(QFile & openFile, qint64 & contentStart, qint64 & contentEnd) {
    // check the size
    qint64 fileSize = openFile.size();
    qint64 footerSize = sizeof(guid) + sizeof(qint64);
    assert(fileSize >= footerSize);

    // read the footer
    openFile.seek(fileSize - footerSize);
    qint64 contentSize;
    openFile.read((char *)&contentSize, sizeof(qint64));
    unsigned char guidBuffer[sizeof(guid)];
    openFile.read((char *)guidBuffer, sizeof(guid));

    // check the guid
    if (memcmp(guidBuffer, guid, sizeof(guid)) != 0) {
        // guid doesn't match. no content.
        contentStart = fileSize;
        contentEnd = fileSize;
        return;
    }

    // check the size (make sure it's in the file)
    assert(fileSize >= contentSize + footerSize);

    // output
    contentEnd = fileSize - footerSize;
    contentStart = contentEnd - contentSize;
}

void ExeParser::copyOnlyExe(QString exeFileSource, QString exeFileDest)
{
    // read source
    QFile in(exeFileSource);
    in.open(QIODevice::ReadOnly);
    qint64 contentStart, contentEnd;
    parse(in, contentStart, contentEnd);

    // open out file
    QFile out(exeFileDest);
    out.open(QIODevice::WriteOnly);

    // copy exe code to out file
    in.seek(0);
    out.write(in.read(contentStart));

    // close
    in.close();
    out.close();
}

QByteArray ExeParser::read(QString exeFile)
{
    QFile in(exeFile);
    in.open(QIODevice::ReadOnly);
    qint64 contentStart, contentEnd;
    parse(in, contentStart, contentEnd);

    in.seek(contentStart);
    QByteArray doc = in.read(contentEnd - contentStart);
    in.close();
    return doc;
}

void ExeParser::write(QString exeFile, QByteArray document)
{
    // if the file doesn't exist, copy this exe to it
    QFile out(exeFile);

    if (! out.exists())
        copyOnlyExe(QApplication::applicationFilePath(), exeFile);

    // overwrite the document
    out.open(QIODevice::ReadWrite);
    qint64 contentStart, contentEnd;
    parse(out, contentStart, contentEnd);

    out.seek(contentStart);
    out.write(document);

    // write footer
    qint64 size = document.size();
    out.write((const char *) &size, sizeof(qint64));
    out.write((const char *) guid, sizeof(guid));

    out.close();

}
