#include "MainWindow.h"
#include "ExeParser.h"

#include <QtCore/QProcess>
#include <QtCore/QDir>
#include <QtCore/QObject>
#include <QtGui/QApplication>

const char * targetParameter = "--target";
QString getTargetExe();
QString copySelfToTemp();

// run this program with --target <exe> to open the file. if this argument
// is not present, it will copy itself minus the document to temp folder and
// run the copy with that argument on itself
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName(QObject::tr("Lao Wai Chinese"));
    QCoreApplication::setOrganizationDomain("laowaichinese.net");
    QCoreApplication::setApplicationName(QObject::tr("Secure WordVault"));
    QCoreApplication::setApplicationVersion("0.1.1");

    // determine whether to run or copy ourself to temp and run that
    QString targetExe = getTargetExe();

    // if we're debugging, just start the app without doing that temp file
    // fanagaling
#ifndef QT_NO_DEBUG
    MainWindow w;
    w.show();
    return app.exec();
#endif

    if (targetExe.isNull()) {
        // this is the user double clicking the file.
        QString tmpExePath = copySelfToTemp();
        QStringList args;
        args.append(targetParameter);
        args.append(QApplication::applicationFilePath());
        QProcess::startDetached(tmpExePath, args);
        return 0;
    } else {
        // this is the tmp file running.
        MainWindow w(targetExe);
        w.show();
        return app.exec();
    }
}

QString getTargetExe() {
    QStringList args = QCoreApplication::arguments();

    int index = args.indexOf(targetParameter);

    if (index == -1 || index >= args.size())
        return QString();

    return args.at(index+1);
}

QString copySelfToTemp() {
    QString selfPath = QApplication::applicationFilePath();

    QString tempTitle = QApplication::applicationName() + QString("-") + QString(QApplication::applicationVersion()) + QString(".exe");
    QString tempPath = QDir::temp().absoluteFilePath(tempTitle);

    ExeParser::copyOnlyExe(selfPath, tempPath);

    return tempPath;
}

