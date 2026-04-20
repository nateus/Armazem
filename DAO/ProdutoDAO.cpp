#include "DAO/ProdutoDAO.h"

#include "Database/DatabaseManager.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

namespace {

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

ProdutoDAO::ProdutoDAO(DatabaseManager &databaseManager)
    : m_databaseManager(databaseManager)
{
}

int ProdutoDAO::insert(const ProdutoRecord &produto)
{
    if (!m_databaseManager.open()) {
        setLastError(m_databaseManager.lastError());
        return 0;
    }

    QSqlQuery query(m_databaseManager.database());
    query.prepare(QStringLiteral(
        "INSERT INTO produtos (codigo, nome, descricao, preco, quantidade, categoria_id, ativo, data_cadastro) "
        "VALUES (:codigo, :nome, :descricao, :preco, :quantidade, :categoria_id, :ativo, :data_cadastro)"));
    bindProduto(query, produto);

    if (!query.exec()) {
        setLastError(query.lastError().text());
        return 0;
    }

    m_lastError.clear();
    return query.lastInsertId().toInt();
}

bool ProdutoDAO::update(const ProdutoRecord &produto)
{
    if (!m_databaseManager.open()) {
        setLastError(m_databaseManager.lastError());
        return false;
    }

    QSqlQuery query(m_databaseManager.database());
    query.prepare(QStringLiteral(
        "UPDATE produtos SET codigo = :codigo, nome = :nome, descricao = :descricao, "
        "preco = :preco, quantidade = :quantidade, categoria_id = :categoria_id, ativo = :ativo "
        "WHERE id = :id"));
    bindProduto(query, produto);
    query.bindValue(QStringLiteral(":id"), produto.id);

    if (!query.exec()) {
        setLastError(query.lastError().text());
        return false;
    }

    m_lastError.clear();
    return query.numRowsAffected() > 0;
}

bool ProdutoDAO::setActive(int produtoId, bool ativo)
{
    if (!m_databaseManager.open()) {
        setLastError(m_databaseManager.lastError());
        return false;
    }

    QSqlQuery query(m_databaseManager.database());
    query.prepare(QStringLiteral("UPDATE produtos SET ativo = :ativo WHERE id = :id"));
    query.bindValue(QStringLiteral(":ativo"), ativo ? 1 : 0);
    query.bindValue(QStringLiteral(":id"), produtoId);

    if (!query.exec()) {
        setLastError(query.lastError().text());
        return false;
    }

    m_lastError.clear();
    return query.numRowsAffected() > 0;
}

std::optional<ProdutoRecord> ProdutoDAO::findById(int produtoId)
{
    if (!m_databaseManager.open()) {
        setLastError(m_databaseManager.lastError());
        return std::nullopt;
    }

    QSqlQuery query(m_databaseManager.database());
    query.prepare(QStringLiteral(
        "SELECT id, codigo, nome, descricao, preco, quantidade, categoria_id, ativo, data_cadastro "
        "FROM produtos WHERE id = :id LIMIT 1"));
    query.bindValue(QStringLiteral(":id"), produtoId);

    if (!query.exec()) {
        setLastError(query.lastError().text());
        return std::nullopt;
    }

    if (!query.next()) {
        m_lastError.clear();
        return std::nullopt;
    }

    m_lastError.clear();
    return mapProduto(query);
}

QVector<ProdutoRecord> ProdutoDAO::search(const QString &filtro, bool onlyActive)
{
    QVector<ProdutoRecord> produtos;

    if (!m_databaseManager.open()) {
        setLastError(m_databaseManager.lastError());
        return produtos;
    }

    QString sql =
        QStringLiteral("SELECT id, codigo, nome, descricao, preco, quantidade, categoria_id, ativo, data_cadastro "
                       "FROM produtos WHERE (:filtro = '' OR codigo LIKE :likeCodigo OR nome LIKE :likeNome)");
    if (onlyActive) {
        sql += QStringLiteral(" AND ativo = 1");
    }
    sql += QStringLiteral(" ORDER BY nome");

    QSqlQuery query(m_databaseManager.database());
    query.prepare(sql);
    query.bindValue(QStringLiteral(":filtro"), filtro.trimmed());
    query.bindValue(QStringLiteral(":likeCodigo"), QStringLiteral("%%1%").arg(filtro.trimmed()));
    query.bindValue(QStringLiteral(":likeNome"), QStringLiteral("%%1%").arg(filtro.trimmed()));

    if (!query.exec()) {
        setLastError(query.lastError().text());
        return produtos;
    }

    while (query.next()) {
        produtos.append(mapProduto(query));
    }

    m_lastError.clear();
    return produtos;
}

QString ProdutoDAO::lastError() const
{
    return m_lastError;
}

ProdutoRecord ProdutoDAO::mapProduto(const QSqlQuery &query) const
{
    ProdutoRecord produto;
    produto.id = query.value(0).toInt();
    produto.codigo = query.value(1).toString();
    produto.nome = query.value(2).toString();
    produto.descricao = query.value(3).toString();
    produto.preco = query.value(4).toDouble();
    produto.quantidade = query.value(5).toInt();
    produto.categoriaId = query.value(6).toInt();
    produto.ativo = query.value(7).toInt() == 1;
    produto.dataCadastro = parseDateTime(query.value(8));
    return produto;
}

void ProdutoDAO::bindProduto(QSqlQuery &query, const ProdutoRecord &produto) const
{
    query.bindValue(QStringLiteral(":codigo"), produto.codigo);
    query.bindValue(QStringLiteral(":nome"), produto.nome);
    query.bindValue(QStringLiteral(":descricao"), produto.descricao);
    query.bindValue(QStringLiteral(":preco"), produto.preco);
    query.bindValue(QStringLiteral(":quantidade"), produto.quantidade);
    if (produto.categoriaId > 0) {
        query.bindValue(QStringLiteral(":categoria_id"), produto.categoriaId);
    } else {
        query.bindValue(QStringLiteral(":categoria_id"), QVariant());
    }
    query.bindValue(QStringLiteral(":ativo"), produto.ativo ? 1 : 0);
    query.bindValue(QStringLiteral(":data_cadastro"), dateTimeToDatabase(produto.dataCadastro));
}

void ProdutoDAO::setLastError(const QString &message)
{
    m_lastError = message;
}
