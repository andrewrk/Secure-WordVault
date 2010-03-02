#include <QtGui/QApplication>
#include "MainWindow.h"

#include "ExeParser.h"

#include <QProcess>
#include <QDir>

QString getTheRealPath(int argc, char *argv[]);
QString copySelf(QString selfPath);
void beExecutingThisOtherFile(QString exePath, QString theRealPath);

int main(int argc, char *argv[])
{
    QString theRealPath = getTheRealPath(argc, argv);
    if (theRealPath == NULL) {
        // this is the user double clicking the file.
        QString selfPath = argv[0];
        QString tmpExePath = copySelf(selfPath);
        beExecutingThisOtherFile(tmpExePath, selfPath);
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

QString getTheRealPath(int argc, char *argv[]) {
    if (argc < 3)
        return NULL;
    QString dashDashTheRealPath = argv[1];
    if (dashDashTheRealPath != "--theRealPath")
        return NULL;
    return argv[2];
}

QString copySelf(QString selfPath) {
    // read self
    QFile selfFile(selfPath);
    selfFile.open(QIODevice::ReadOnly);
    qint64 contentStart, contentEnd;
    ExeParser::parse(selfFile, contentStart, contentEnd);

    // open tmp file
    QDir tmpDir = QDir::temp();
    QString tmpExePath = tmpDir.absoluteFilePath("Secure WordVault.exe");
    QFile tmpExeFile(tmpExePath);
    tmpExeFile.open(QIODevice::WriteOnly);

    // copy exe code to tmp file
    selfFile.seek(0);
    char * exeData = new char[contentStart];
    selfFile.read(exeData, contentStart);
    tmpExeFile.write(exeData, contentStart);

    // close
    selfFile.close();
    tmpExeFile.close();

    return tmpExePath;
}

void beExecutingThisOtherFile(QString exePath, QString theRealPath) {
    QStringList args;
    args.append("--theRealFile");
    args.append(theRealPath);
    QProcess::startDetached(exePath, args);
}
