#include "DAO/UsuarioDAO.h"

#include "Database/DatabaseManager.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

namespace {

QString tipoUsuarioToDatabase(TipoUsuario tipo)
{
    switch (tipo) {
    case TipoUsuario::Admin:
        return QStringLiteral("admin");
    case TipoUsuario::Gerente:
        return QStringLiteral("gerente");
    case TipoUsuario::Funcionario:
        return QStringLiteral("funcionario");
    case TipoUsuario::Invalido:
        break;
    }

    return QStringLiteral("funcionario");
}

TipoUsuario tipoUsuarioFromDatabase(const QString &tipo)
{
    const QString normalized = tipo.toLower();
    if (normalized == QStringLiteral("admin")) {
        return TipoUsuario::Admin;
    }
    if (normalized == QStringLiteral("gerente")) {
        return TipoUsuario::Gerente;
    }
    if (normalized == QStringLiteral("funcionario")) {
        return TipoUsuario::Funcionario;
    }

    return TipoUsuario::Invalido;
}

QDateTime parseDateTime(const QVariant &value)
{
    QDateTime dateTime = QDateTime::fromString(value.toString(), Qt::ISODate);
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(value.toString(), QStringLiteral("yyyy-MM-dd HH:mm:ss"));
    }
    return dateTime;
}

QString dateTimeToDatabase(const QDateTime &dateTime)
{
    const QDateTime value = dateTime.isValid() ? dateTime : QDateTime::currentDateTime();
    return value.toString(Qt::ISODate);
}

} // namespace

UsuarioDAO::UsuarioDAO(DatabaseManager &databaseManager)
    : m_databaseManager(databaseManager)
{
}

bool UsuarioDAO::existsAdmin()
{
    if (!m_databaseManager.open()) {
        setLastError(m_databaseManager.lastError());
        return false;
    }

    QSqlQuery query(m_databaseManager.database());
    query.prepare(QStringLiteral("SELECT 1 FROM usuarios WHERE tipo = :tipo LIMIT 1"));
    query.bindValue(QStringLiteral(":tipo"), QStringLiteral("admin"));

    if (!query.exec()) {
        setLastError(query.lastError().text());
        return false;
    }

    m_lastError.clear();
    return query.next();
}

std::optional<Usuario> UsuarioDAO::findByLogin(const QString &login)
{
    if (!m_databaseManager.open()) {
        setLastError(m_databaseManager.lastError());
        return std::nullopt;
    }

    QSqlQuery query(m_databaseManager.database());
    query.prepare(QStringLiteral(
        "SELECT id, nome, login, senha_hash, tipo, ativo, data_criacao "
        "FROM usuarios WHERE login = :login LIMIT 1"));
    query.bindValue(QStringLiteral(":login"), login);

    if (!query.exec()) {
        setLastError(query.lastError().text());
        return std::nullopt;
    }

    if (!query.next()) {
        m_lastError.clear();
        return std::nullopt;
    }

    m_lastError.clear();
    return mapUsuario(query);
}

int UsuarioDAO::insert(const Usuario &usuario)
{
    if (!m_databaseManager.open()) {
        setLastError(m_databaseManager.lastError());
        return 0;
    }

    QSqlQuery query(m_databaseManager.database());
    query.prepare(QStringLiteral(
        "INSERT INTO usuarios (nome, login, senha_hash, tipo, ativo, data_criacao) "
        "VALUES (:nome, :login, :senha_hash, :tipo, :ativo, :data_criacao)"));
    query.bindValue(QStringLiteral(":nome"), usuario.getNome());
    query.bindValue(QStringLiteral(":login"), usuario.getLogin());
    query.bindValue(QStringLiteral(":senha_hash"), usuario.getSenhaHash());
    query.bindValue(QStringLiteral(":tipo"), tipoUsuarioToDatabase(usuario.getTipo()));
    query.bindValue(QStringLiteral(":ativo"), usuario.isAtivo() ? 1 : 0);
    query.bindValue(QStringLiteral(":data_criacao"), dateTimeToDatabase(usuario.getDataCriacao()));

    if (!query.exec()) {
        setLastError(query.lastError().text());
        return 0;
    }

    m_lastError.clear();
    return query.lastInsertId().toInt();
}

bool UsuarioDAO::update(const Usuario &usuario)
{
    if (!m_databaseManager.open()) {
        setLastError(m_databaseManager.lastError());
        return false;
    }

    QSqlQuery query(m_databaseManager.database());
    query.prepare(QStringLiteral(
        "UPDATE usuarios SET nome = :nome, login = :login, senha_hash = :senha_hash, "
        "tipo = :tipo, ativo = :ativo WHERE id = :id"));
    query.bindValue(QStringLiteral(":nome"), usuario.getNome());
    query.bindValue(QStringLiteral(":login"), usuario.getLogin());
    query.bindValue(QStringLiteral(":senha_hash"), usuario.getSenhaHash());
    query.bindValue(QStringLiteral(":tipo"), tipoUsuarioToDatabase(usuario.getTipo()));
    query.bindValue(QStringLiteral(":ativo"), usuario.isAtivo() ? 1 : 0);
    query.bindValue(QStringLiteral(":id"), usuario.getId());

    if (!query.exec()) {
        setLastError(query.lastError().text());
        return false;
    }

    m_lastError.clear();
    return query.numRowsAffected() > 0;
}

bool UsuarioDAO::setActive(int usuarioId, bool ativo)
{
    if (!m_databaseManager.open()) {
        setLastError(m_databaseManager.lastError());
        return false;
    }

    QSqlQuery query(m_databaseManager.database());
    query.prepare(QStringLiteral("UPDATE usuarios SET ativo = :ativo WHERE id = :id"));
    query.bindValue(QStringLiteral(":ativo"), ativo ? 1 : 0);
    query.bindValue(QStringLiteral(":id"), usuarioId);

    if (!query.exec()) {
        setLastError(query.lastError().text());
        return false;
    }

    m_lastError.clear();
    return query.numRowsAffected() > 0;
}

QVector<Usuario> UsuarioDAO::list(bool includeInactive)
{
    QVector<Usuario> usuarios;

    if (!m_databaseManager.open()) {
        setLastError(m_databaseManager.lastError());
        return usuarios;
    }

    QSqlQuery query(m_databaseManager.database());
    const QString sql = includeInactive
        ? QStringLiteral("SELECT id, nome, login, senha_hash, tipo, ativo, data_criacao FROM usuarios ORDER BY nome")
        : QStringLiteral("SELECT id, nome, login, senha_hash, tipo, ativo, data_criacao FROM usuarios WHERE ativo = 1 ORDER BY nome");
    query.prepare(sql);

    if (!query.exec()) {
        setLastError(query.lastError().text());
        return usuarios;
    }

    while (query.next()) {
        usuarios.append(mapUsuario(query));
    }

    m_lastError.clear();
    return usuarios;
}

QString UsuarioDAO::lastError() const
{
    return m_lastError;
}

Usuario UsuarioDAO::mapUsuario(const QSqlQuery &query) const
{
    Usuario usuario;
    usuario.setId(query.value(0).toInt());
    usuario.setNome(query.value(1).toString());
    usuario.setLogin(query.value(2).toString());
    usuario.setSenhaHash(query.value(3).toString());
    usuario.setTipo(tipoUsuarioFromDatabase(query.value(4).toString()));
    usuario.setAtivo(query.value(5).toInt() == 1);
    usuario.setDataCriacao(parseDateTime(query.value(6)));
    return usuario;
}

void UsuarioDAO::setLastError(const QString &message)
{
    m_lastError = message;
}
