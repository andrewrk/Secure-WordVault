#include "Encryption.h"

QString Encryption::decrypted(QByteArray document, QString password, bool *ok)
{
    // TODO replace this with the actual decryption algorithm
    return QString(document);
}

QByteArray Encryption::encrypted(QString document, QString password)
{
    // TODO replace this with the actual encryption algorithm
    QByteArray out;
    out.append(document);
    return out;
}
