#include "ExeParser.h"

// generated from http://www.guidgenerator.com/online-guid-generator.aspx
const unsigned char guid[] = {0x7c, 0x7a, 0x64, 0x0e, 0x9b, 0x4f, 0x42, 0x58, 0xa8, 0xb4, 0xd8, 0x4b, 0xd5, 0x8a, 0x05, 0x14};

// TODO proper assertion failure
void omgWtf() { throw 1; }

void ExeParser::parse(QFile & openFile, qint64 & contentStart, qint64 & contentEnd) {
    // check the size
    qint64 fileSize = openFile.size();
    qint64 footerSize = sizeof(guid) + sizeof(qint64);
    if (fileSize < footerSize)
        omgWtf(); // file is too small

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
    if (fileSize < contentSize + footerSize)
        omgWtf(); // contentSize is too big

    // output
    contentEnd = fileSize - footerSize;
    contentStart = contentEnd - contentSize;
}

