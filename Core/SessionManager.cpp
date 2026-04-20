#include "SessionManager.h"

void SessionManager::loginUser(int id, const QString& nome, const QString& login, const QString& tipo) {
    currentUserId = id;
    currentNome = nome;
    currentLogin = login;
    currentTipo = tipo;
    loggedIn = true;
}

void SessionManager::logoutUser() {
    currentUserId = -1;
    currentNome = "";
    currentLogin = "";
    currentTipo = "";
    loggedIn = false;
}

bool SessionManager::isUserLoggedIn() const {
    return loggedIn;
}

int SessionManager::getUserId() const { return currentUserId; }
QString SessionManager::getNome() const { return currentNome; }
QString SessionManager::getTipo() const { return currentTipo; }
