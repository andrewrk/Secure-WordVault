#include <QtGui/QApplication>
#include <QtCore/QObject>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName(QObject::tr("Lao Wai Chinese"));
    QCoreApplication::setOrganizationDomain("laowaichinese.net");
    QCoreApplication::setApplicationName(QObject::tr("Secure WordVault"));

    // TODO: replace the argument of MainWindow with the command line parameter
    MainWindow w(QApplication::applicationFilePath());
    w.show();
    return a.exec();
}
