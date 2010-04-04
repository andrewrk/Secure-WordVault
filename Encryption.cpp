#include "Encryption.h"
#include <openssl/evp.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


///////////////////////////////////////
//
// initialize Encryption and decryption context
// using OpenSSL
//
int INITIALIZEAES(
unsigned char *KeyData,
int KeyLength,
unsigned char *Salt,
EVP_CIPHER_CTX *en_ctx, EVP_CIPHER_CTX *de_ctx)
{
int rc;
int Rounds = 10;
unsigned char aes_key[32], aes_iv[32];

///////////////////////////////////////////
//
//
// Cipher Block chaining mode , SHA1
rc = EVP_BytesToKey(
EVP_aes_256_cbc(), // Cryptographic mode
EVP_sha1(), // SHA1
Salt, // a fuzzifier
KeyData,
KeyLength,
Rounds, // more rounds
aes_key, aes_iv); // return buffers

EVP_CIPHER_CTX_init(en_ctx);
EVP_EncryptInit_ex(en_ctx,
EVP_aes_256_cbc(),
NULL,
aes_key,
aes_iv);

EVP_CIPHER_CTX_init(de_ctx);
EVP_DecryptInit_ex(de_ctx,
EVP_aes_256_cbc(),
NULL, aes_key, aes_iv);

return 0;

}

#define AES_BLOCK_SIZE 128

unsigned char *Encrypt(
EVP_CIPHER_CTX *en_ctx,
unsigned char *msg,
int *msglen)
{
int buffer_length = *msglen + AES_BLOCK_SIZE;
int final_length = 0;
unsigned char *ciphermsg = (unsigned char *)malloc(buffer_length );

EVP_EncryptInit_ex(en_ctx, NULL, NULL, NULL, NULL);
EVP_EncryptUpdate(en_ctx, ciphermsg, &buffer_length, msg, *msglen);
EVP_EncryptFinal_ex(en_ctx, ciphermsg+buffer_length, &final_length);
*msglen = buffer_length + final_length;
return ciphermsg;
}

unsigned char *DeCrypt(EVP_CIPHER_CTX *de_ctx,
unsigned char *ciphertext,
int *len)
{
int p_len = *len, f_len = 0;
unsigned char *plaintext =(unsigned char *)malloc(p_len);

EVP_DecryptInit_ex(de_ctx, NULL, NULL, NULL, NULL);
EVP_DecryptUpdate(de_ctx, plaintext, &p_len, ciphertext, *len); EVP_DecryptFinal_ex(de_ctx, plaintext+p_len, &f_len);
*len = p_len + f_len;
return plaintext;
}




QString Encryption::decrypted(QByteArray document, QString password, bool *ok)
{
    EVP_CIPHER_CTX en_ctx;
    EVP_CIPHER_CTX de_ctx;
    EVP_MD_CTX mdctx;

    unsigned char md_value[EVP_MAX_MD_SIZE], stored_md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len;

    unsigned int Salt[] = {12345, 54321};
    unsigned char *KeyData, *DocData;
    int KeyDataLen, DocDataLength;
    int outlen = document.length() + 1;

    KeyData = (unsigned char *)password.toLatin1().constData();
    KeyDataLen = password.length();
    DocData = (unsigned char *)document.constData();
    DocDataLength = document.length();

    EVP_DigestInit(&mdctx, EVP_md5());
    EVP_DigestUpdate(&mdctx, KeyData, (size_t) KeyDataLen);
    EVP_DigestFinal_ex(&mdctx, md_value, &md_len);
    EVP_MD_CTX_cleanup(&mdctx);

    memmove(stored_md_value, DocData + DocDataLength - 1 - md_len, md_len);
    DocDataLength -= md_len;

    *ok = true;
    if (memcmp(stored_md_value, md_value, md_len))
        *ok = false;

    INITIALIZEAES(KeyData, KeyDataLen, (unsigned char *)&Salt, &en_ctx, &de_ctx);

    char *msg = (char *)DeCrypt(&de_ctx, DocData , &outlen);

    // TODO replace this with the actual decryption algorithm
    return QString(msg);
}

QByteArray Encryption::encrypted(QString document, QString password)
{

    EVP_CIPHER_CTX en_ctx;
    EVP_CIPHER_CTX de_ctx;
    EVP_MD_CTX mdctx;

    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len;

    unsigned int Salt[] = {12345, 54321};
    unsigned char *KeyData;
    int KeyDataLen;

    char *msgtoencrypt = (char *)document.toLatin1().constData();

    KeyData = (unsigned char *)password.toLatin1().constData();
    KeyDataLen = password.length();

    EVP_DigestInit(&mdctx, EVP_md5());
    EVP_DigestUpdate(&mdctx, KeyData, (size_t) KeyDataLen);
    EVP_DigestFinal_ex(&mdctx, md_value, &md_len);
    EVP_MD_CTX_cleanup(&mdctx);

    INITIALIZEAES(KeyData, KeyDataLen, (unsigned char *)&Salt, &en_ctx, &de_ctx);

    unsigned char *ciphermsg;
    int outlen, inlen;

    outlen = inlen = strlen(msgtoencrypt)+1;

    ciphermsg = Encrypt(&en_ctx, (unsigned char *)msgtoencrypt, &inlen);

    QByteArray out;
    out.append((char *)ciphermsg);
    out.append((char *)md_value);
    return out;
}
