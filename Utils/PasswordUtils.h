#pragma once

#include <QString>

#include <QCryptographicHash>

class PasswordUtils
{
public:
    static QString hashPassword(const QString &plainText)
    {
        return QString::fromLatin1(
            QCryptographicHash::hash(plainText.toUtf8(), QCryptographicHash::Sha256).toHex());
    }

    static bool verifyPassword(const QString &plainText, const QString &hash)
    {
        return hashPassword(plainText) == hash;
    }
};
