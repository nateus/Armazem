#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QString>

class SessionManager {
public:
    static SessionManager& instance() {
        static SessionManager instance;
        return instance;
    }

    void loginUser(int id, const QString& nome, const QString& login, const QString& tipo);
    void logoutUser();
    bool isUserLoggedIn() const;

    // Getters para a sessão atual
    int getUserId() const;
    QString getNome() const;
    QString getTipo() const; // 'admin', 'gerente' ou 'funcionario'

private:
    SessionManager() = default; // Construtor privado (Singleton)
    
    bool loggedIn = false;
    int currentUserId = -1;
    QString currentNome;
    QString currentLogin;
    QString currentTipo;
};

#endif // SESSIONMANAGER_H
