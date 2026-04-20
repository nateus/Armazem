#include "AuthService.h"
#include "../Core/SessionManager.h"
#include <QDebug>

// Assumindo a interface do colega do pacote 3:
// #include "DAO/UsuarioDAO.h"
// #include "Models/Usuario.h"
// Assumindo pacote 2:
// #include "Utils/PasswordUtils.h"

AuthService::AuthService(UsuarioDAO* usuarioDao) 
    : m_usuarioDao(usuarioDao) {}

bool AuthService::checkAdminExists() const {
    // Chamada ao pacote 3
    // return m_usuarioDao->existsAdmin();
    return false; // Placeholder para compilar
}

bool AuthService::registerInitialAdmin(const QString& senha) {
    if (checkAdminExists()) {
        return false; // Já existe, não pode criar outro admin inicial
    }

    // Regra: Senha nunca em texto puro
    // QString hash = PasswordUtils::hashPassword(senha);
    QString hash = "hash_falso_aqui"; // Placeholder
    
    // Regra: Criar usuário admin
    // Usuario novoAdmin(0, "Administrador Principal", "admin", hash, "admin", true);
    // return m_usuarioDao->insert(novoAdmin);
    return true; // Placeholder
}

bool AuthService::login(const QString& loginStr, const QString& senha, QString& outErrorMessage) {
    // 1. Busca o usuário pelo login (Pacote 3)
    // Usuario user = m_usuarioDao->findByLogin(loginStr);
    
    /* Pseudocódigo baseando-se no DAO:
    if (user.id == 0) {
        outErrorMessage = "Usuário não encontrado.";
        return false;
    }

    // 2. Bloquear usuário inativo (Regra Obrigatória)
    if (!user.ativo) {
        outErrorMessage = "Usuário inativo. Acesso bloqueado.";
        return false;
    }

    // 3. Validar hash (Pacote 2)
    if (!PasswordUtils::verifyPassword(senha, user.senha_hash)) {
        outErrorMessage = "Senha incorreta.";
        return false;
    }

    // 4. Iniciar Sessão
    SessionManager::instance().loginUser(user.id, user.nome, user.login, user.tipo);
    return true;
    */

    outErrorMessage = "Funcionalidade aguardando Pacotes 2 e 3.";
    return false;
}

void AuthService::logout() {
    SessionManager::instance().logoutUser();
}
