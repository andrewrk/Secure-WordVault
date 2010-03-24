#include "Encryption.h"
#include "aescpp.h"

QString Encryption::decrypted(QByteArray document, QString password, bool *ok)
{
    const char * doc = document.constData();
    char * output = new char[strlen(doc) * 2];
    memset(output, 0, strlen(doc)*2);
    unsigned char * iv = new unsigned char[16];
    for (int i = 0; i < 16; i++)
        iv[i] = rand();
    aes_init();
    aes_encrypt_ctx cx;
    aes_encrypt_key256((const unsigned char *)password.toLatin1().constData(), &cx);
    aes_cfb_decrypt((const unsigned char *)doc, (unsigned char *)output, strlen(doc) + 1, iv, &cx);

    *ok = true;
    return QString(output);
}

QByteArray Encryption::encrypted(QString document, QString password)
{
    const char * doc = document.toLatin1().constData();
    char * output = new char[strlen(doc)];
    memset(output, 0, strlen(doc)*2);
    unsigned char * iv = new unsigned char[16];
    for (int i = 0; i < 16; i++)
        iv[i] = rand();
    aes_init();
    aes_encrypt_ctx cx;
    aes_encrypt_key256((const unsigned char *)password.toLatin1().constData(), &cx);
    aes_cfb_encrypt((const unsigned char *)doc, (unsigned char *)output, strlen(doc) + 1, iv, &cx);

    QByteArray out;
    out.append(output);
    return out;
}
