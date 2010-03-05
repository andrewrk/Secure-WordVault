#include "Encryption.h"

QString Encryption::decrypted(QByteArray document, QString password, bool *ok)
{
    // TODO replace this with the actual decryption algorithm
    *ok = true;
    return QString(document);
}

QByteArray Encryption::encrypted(QString document, QString password)
{
    // TODO replace this with the actual encryption algorithm
    QByteArray out;
    out.append(document);
    return out;
}
