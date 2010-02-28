#include <QtGui/QApplication>
#include "MainWindow.h"

char * getTheRealPath(int argc, char *argv[]);
void copySelf();

int main(int argc, char *argv[])
{
    char * theRealPath = getTheRealPath(argc, argv);
    if (theRealPath == NULL) {
        copySelf();
        return 0;
    } else {
        // load application
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

void copySelf() {
    // TODO
}
