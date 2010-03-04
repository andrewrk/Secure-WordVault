#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <QByteArray>
#include <QString>

// static class that encrypts / decrypts data using a password and some
// algorithm. it has a little bit of metadata to tell if the password was
// correct when decrypting.
class Encryption
{
public:
    // returns document encrypted with password
    static QByteArray encrypted(QString document, QString password);

    // returns document decrypted with password. if ok is supplied, sets it
    // to whether or not the entered password was correct.
    static QString decrypted(QByteArray document, QString password, bool * ok = NULL);
};

#endif // ENCRYPTION_H
