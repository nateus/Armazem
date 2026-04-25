#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include <QString>

#include <memory>

#include "../DAO/UsuarioDAO.h"

class AuthService
{
public:
    explicit AuthService(const std::shared_ptr<UsuarioDAO> &usuarioDao);

    bool checkAdminExists();
    bool registerInitialAdmin(const QString &senha);
    bool login(const QString &login, const QString &senha, QString &outErrorMessage);
    void logout();
    QString lastError() const;

private:
    void setLastError(const QString &message);

    std::shared_ptr<UsuarioDAO> m_usuarioDao;
    QString m_lastError;
};

#endif // AUTHSERVICE_H
