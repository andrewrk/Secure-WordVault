#include <QtGui/QApplication>
#include "MainWindow.h"

#include "ExeParser.h"

char * getTheRealPath(int argc, char *argv[]);
void copySelf(char * selfPath);

int main(int argc, char *argv[])
{
    char * theRealPath = getTheRealPath(argc, argv);
    if (theRealPath == NULL) {
        // this is the user double clicking the file.
        copySelf(argv[0]);
        return 0;
    } else {
        // this is the tmp file running.
        // TODO: use theRealPath
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
    }
}

char * getTheRealPath(int argc, char *argv[]) {
    if (argc < 3)
        return NULL;
    char * dashDashTheRealPath = argv[1];
    if (strcmp(dashDashTheRealPath, "--theRealPath") != 0)
        return NULL;
    return argv[2];
}

void copySelf(char * selfPath) {
    std::ios::pos_type asf, asfs;
    ExeParser::parse(selfPath, asf, asfs);
    // TODO
}
