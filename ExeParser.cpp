#include "ExeParser.h"

// generated from http://www.guidgenerator.com/online-guid-generator.aspx
const unsigned char guid[] = {0x7c, 0x7a, 0x64, 0x0e, 0x9b, 0x4f, 0x42, 0x58, 0xa8, 0xb4, 0xd8, 0x4b, 0xd5, 0x8a, 0x05, 0x14};

// TODO proper assertion failure
void omgWtf() { throw 1; }

void ExeParser::parse(char * path, std::ios::pos_type & contentStart, std::ios::pos_type & contentEnd) {
    // the "ate" flag puts the cursor at the end of the file, which is apparently the proper way to get the size of a file.
    std::ifstream handle(path, std::ios::in | std::ios::binary | std::ios::ate);
    if (!handle.is_open())
        omgWtf();

    // get the size
    std::ios::pos_type fileSize = handle.tellg();
    int footerSize = sizeof(guid) + sizeof(int);
    if (fileSize < footerSize)
        omgWtf(); // file is too small

    // read the footer
    handle.seekg((std::ios::off_type)-footerSize, std::ios::end);
    int contentSize;
    handle.read((char *)&contentSize, sizeof(int));
    char guidBuffer[sizeof(guid)];
    handle.read(guidBuffer, sizeof(guid));

    // check the guid
    if (memcmp(guidBuffer, guid, sizeof(guid)) != 0) {
        // guid doesn't match. no content.
        contentStart = 0;
        contentEnd = 0;
        return;
    }

    // check the size (make sure it's in the file)
    if (fileSize < contentSize + footerSize)
        omgWtf(); // contentSize is too big

    // HOLY TYPE CASTING BATMAN!!!
    contentEnd = (std::ios::pos_type)((std::ios::off_type)fileSize - (std::ios::off_type)footerSize);
    contentStart = (std::ios::pos_type)((std::ios::off_type)contentEnd - (std::ios::off_type)contentSize);
}

