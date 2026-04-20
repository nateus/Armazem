#include "DAO/CategoriaDAO.h"

#include "Database/DatabaseManager.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

CategoriaDAO::CategoriaDAO(DatabaseManager &databaseManager)
    : m_databaseManager(databaseManager)
{
}

QVector<CategoriaRecord> CategoriaDAO::listAll()
{
    QVector<CategoriaRecord> categorias;

    if (!m_databaseManager.open()) {
        setLastError(m_databaseManager.lastError());
        return categorias;
    }

    QSqlQuery query(m_databaseManager.database());
    query.prepare(QStringLiteral("SELECT id, nome, descricao FROM categorias ORDER BY nome"));

    if (!query.exec()) {
        setLastError(query.lastError().text());
        return categorias;
    }

    while (query.next()) {
        categorias.append(mapCategoria(query));
    }

    m_lastError.clear();
    return categorias;
}

std::optional<CategoriaRecord> CategoriaDAO::findByName(const QString &nome)
{
    if (!m_databaseManager.open()) {
        setLastError(m_databaseManager.lastError());
        return std::nullopt;
    }

    QSqlQuery query(m_databaseManager.database());
    query.prepare(QStringLiteral("SELECT id, nome, descricao FROM categorias WHERE nome = :nome LIMIT 1"));
    query.bindValue(QStringLiteral(":nome"), nome);

    if (!query.exec()) {
        setLastError(query.lastError().text());
        return std::nullopt;
    }

    if (!query.next()) {
        m_lastError.clear();
        return std::nullopt;
    }

    m_lastError.clear();
    return mapCategoria(query);
}

int CategoriaDAO::createIfMissing(const QString &nome, const QString &descricao)
{
    const std::optional<CategoriaRecord> existing = findByName(nome);
    if (existing.has_value()) {
        return existing->id;
    }

    if (!m_databaseManager.open()) {
        setLastError(m_databaseManager.lastError());
        return 0;
    }

    QSqlQuery query(m_databaseManager.database());
    query.prepare(QStringLiteral("INSERT INTO categorias (nome, descricao) VALUES (:nome, :descricao)"));
    query.bindValue(QStringLiteral(":nome"), nome);
    query.bindValue(QStringLiteral(":descricao"), descricao);

    if (!query.exec()) {
        setLastError(query.lastError().text());
        return 0;
    }

    m_lastError.clear();
    return query.lastInsertId().toInt();
}

QString CategoriaDAO::lastError() const
{
    return m_lastError;
}

CategoriaRecord CategoriaDAO::mapCategoria(const QSqlQuery &query) const
{
    CategoriaRecord categoria;
    categoria.id = query.value(0).toInt();
    categoria.nome = query.value(1).toString();
    categoria.descricao = query.value(2).toString();
    return categoria;
}

void CategoriaDAO::setLastError(const QString &message)
{
    m_lastError = message;
}
