#include "UserService.h"
#include "../Core/SessionManager.h"
// #include "../Utils/PasswordUtils.h" // Do Pacote 2
// #include "../DAO/UsuarioDAO.h"      // Do Pacote 3

UserService::UserService(UsuarioDAO* usuarioDao) : m_usuarioDao(usuarioDao) {}

bool UserService::isAdminLogado() const {
    return SessionManager::instance().isUserLoggedIn() && 
           SessionManager::instance().getTipo() == "admin";
}

bool UserService::cadastrarUsuario(const QString& nome, const QString& login, const QString& senha, const QString& tipo, bool ativo, QString& errorMessage) {
    if (!isAdminLogado()) {
        errorMessage = "Acesso negado: Apenas administradores podem cadastrar usuários.";
        return false;
    }

    if (nome.isEmpty() || login.isEmpty() || senha.isEmpty() || tipo.isEmpty()) {
        errorMessage = "Todos os campos são obrigatórios.";
        return false;
    }

    // AQUI ENTRA O PACOTE 2 e 3
    // 1. Verificar se login já existe via DAO
    // 2. QString hash = PasswordUtils::hashPassword(senha);
    // 3. m_usuarioDao->insert(...)
    
    return true; // Placeholder
}

bool UserService::editarUsuario(int id, const QString& nome, const QString& login, const QString& tipo, QString& errorMessage) {
    if (!isAdminLogado()) {
        errorMessage = "Acesso negado.";
        return false;
    }
    // Lógica do DAO aqui (m_usuarioDao->update(...))
    return true;
}

bool UserService::ativarDesativarUsuario(int id, bool novoStatus, QString& errorMessage) {
    if (!isAdminLogado()) {
        errorMessage = "Acesso negado.";
        return false;
    }
    // Impede que o admin desative a si mesmo
    if (id == SessionManager::instance().getUserId() && !novoStatus) {
        errorMessage = "Você não pode desativar sua própria conta.";
        return false;
    }
    // m_usuarioDao->setActive(id, novoStatus);
    return true;
}

bool UserService::redefinirSenha(int id, const QString& novaSenha, QString& errorMessage) {
    if (!isAdminLogado()) {
        errorMessage = "Acesso negado.";
        return false;
    }
    if (novaSenha.isEmpty()) {
        errorMessage = "A nova senha não pode estar vazia.";
        return false;
    }
    // QString novoHash = PasswordUtils::hashPassword(novaSenha);
    // m_usuarioDao->updatePassword(id, novoHash);
    return true;
}

std::vector<Usuario> UserService::listarUsuarios(const QString& filtroTipo, const QString& buscaTexto) {
    std::vector<Usuario> lista;
    if (!isAdminLogado()) return lista;

    // m_usuarioDao->search(filtroTipo, buscaTexto);
    // Placeholder para teste visual:
    lista.push_back({1, "Admin", "admin", "admin", true});
    lista.push_back({2, "João Gerente", "joao.g", "gerente", true});
    lista.push_back({3, "Maria Func", "maria.f", "funcionario", false});
    return lista;
}
