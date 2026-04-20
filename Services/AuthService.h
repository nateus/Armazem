#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include <QString>
// Headers assumidos dos pacotes 2 e 3:
// #include "DAO/UsuarioDAO.h"
// #include "Utils/PasswordUtils.h"

// Forward declaration para compilar sem o arquivo real do DAO agora
class UsuarioDAO; 

class AuthService {
public:
    // Recebe o DAO por injeção de dependência
    explicit AuthService(UsuarioDAO* usuarioDao);

    bool checkAdminExists() const;
    bool registerInitialAdmin(const QString& senha); // Assumiremos o login 'admin'
    bool login(const QString& login, const QString& senha, QString& outErrorMessage);
    void logout();

private:
    UsuarioDAO* m_usuarioDao;
};

#endif // AUTHSERVICE_H
