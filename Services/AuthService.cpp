#include "AuthService.h"

#include "../Core/SessionManager.h"
#include "../Models/Enums.h"
#include "../Models/Usuario.h"
#include "../Utils/PasswordUtils.h"

#include <QDateTime>

AuthService::AuthService(const std::shared_ptr<UsuarioDAO> &usuarioDao)
    : m_usuarioDao(usuarioDao)
{
}

bool AuthService::checkAdminExists()
{
    if (!m_usuarioDao) {
        setLastError(QStringLiteral("Servico de autenticacao sem acesso ao DAO de usuarios."));
        return false;
    }

    const bool exists = m_usuarioDao->existsAdmin();
    if (!exists) {
        setLastError(m_usuarioDao->lastError());
    } else {
        m_lastError.clear();
    }

    return exists;
}

bool AuthService::registerInitialAdmin(const QString &senha)
{
    if (!m_usuarioDao) {
        setLastError(QStringLiteral("Servico de autenticacao sem acesso ao DAO de usuarios."));
        return false;
    }

    if (senha.trimmed().isEmpty()) {
        setLastError(QStringLiteral("A senha do administrador nao pode ficar vazia."));
        return false;
    }

    if (checkAdminExists()) {
        setLastError(QStringLiteral("Ja existe um administrador cadastrado."));
        return false;
    }

    if (!m_lastError.isEmpty()) {
        return false;
    }

    Usuario novoAdmin;
    novoAdmin.setNome(QStringLiteral("Administrador Principal"));
    novoAdmin.setLogin(QStringLiteral("admin"));
    novoAdmin.setSenhaHash(PasswordUtils::hashPassword(senha));
    novoAdmin.setTipo(TipoUsuario::Admin);
    novoAdmin.setAtivo(true);
    novoAdmin.setDataCriacao(QDateTime::currentDateTime());

    const int insertedId = m_usuarioDao->insert(novoAdmin);
    if (insertedId <= 0) {
        setLastError(m_usuarioDao->lastError().isEmpty()
                         ? QStringLiteral("Falha ao registrar o administrador inicial.")
                         : m_usuarioDao->lastError());
        return false;
    }

    m_lastError.clear();
    return true;
}

bool AuthService::login(const QString &login, const QString &senha, QString &outErrorMessage)
{
    if (!m_usuarioDao) {
        outErrorMessage = QStringLiteral("Servico de autenticacao indisponivel.");
        setLastError(outErrorMessage);
        return false;
    }

    const auto usuario = m_usuarioDao->findByLogin(login.trimmed());
    if (!usuario.has_value()) {
        outErrorMessage = m_usuarioDao->lastError().isEmpty()
            ? QStringLiteral("Usuario nao encontrado.")
            : m_usuarioDao->lastError();
        setLastError(outErrorMessage);
        return false;
    }

    if (!usuario->isAtivo()) {
        outErrorMessage = QStringLiteral("Usuario inativo. Acesso bloqueado.");
        setLastError(outErrorMessage);
        return false;
    }

    if (!PasswordUtils::verifyPassword(senha, usuario->getSenhaHash())) {
        outErrorMessage = QStringLiteral("Senha incorreta.");
        setLastError(outErrorMessage);
        return false;
    }

    SessionManager::instance().loginUser(
        usuario->getId(),
        usuario->getNome(),
        usuario->getLogin(),
        EnumConverter::tipoUsuarioToString(usuario->getTipo()));

    m_lastError.clear();
    return true;
}

void AuthService::logout()
{
    SessionManager::instance().logoutUser();
    m_lastError.clear();
}

QString AuthService::lastError() const
{
    return m_lastError;
}

void AuthService::setLastError(const QString &message)
{
    m_lastError = message;
}
