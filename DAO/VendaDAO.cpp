#include "DAO/VendaDAO.h"

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

VendaDAO::VendaDAO(DatabaseManager &databaseManager)
    : m_databaseManager(databaseManager)
{
}

int VendaDAO::insertVenda(const VendaRecord &venda)
{
    if (!m_databaseManager.open()) {
        setLastError(m_databaseManager.lastError());
        return 0;
    }

    QSqlQuery query(m_databaseManager.database());
    query.prepare(QStringLiteral(
        "INSERT INTO vendas (funcionario_id, data_venda, valor_total) "
        "VALUES (:funcionario_id, :data_venda, :valor_total)"));
    query.bindValue(QStringLiteral(":funcionario_id"), venda.funcionarioId);
    query.bindValue(QStringLiteral(":data_venda"), dateTimeToDatabase(venda.dataVenda));
    query.bindValue(QStringLiteral(":valor_total"), venda.valorTotal);

    if (!query.exec()) {
        setLastError(query.lastError().text());
        return 0;
    }

    m_lastError.clear();
    return query.lastInsertId().toInt();
}

int VendaDAO::insertItem(const ItemVendaRecord &item)
{
    if (!m_databaseManager.open()) {
        setLastError(m_databaseManager.lastError());
        return 0;
    }

    QSqlQuery query(m_databaseManager.database());
    query.prepare(QStringLiteral(
        "INSERT INTO itens_venda (venda_id, produto_id, quantidade, preco_unitario, subtotal) "
        "VALUES (:venda_id, :produto_id, :quantidade, :preco_unitario, :subtotal)"));
    query.bindValue(QStringLiteral(":venda_id"), item.vendaId);
    query.bindValue(QStringLiteral(":produto_id"), item.produtoId);
    query.bindValue(QStringLiteral(":quantidade"), item.quantidade);
    query.bindValue(QStringLiteral(":preco_unitario"), item.precoUnitario);
    query.bindValue(QStringLiteral(":subtotal"), item.subtotal);

    if (!query.exec()) {
        setLastError(query.lastError().text());
        return 0;
    }

    m_lastError.clear();
    return query.lastInsertId().toInt();
}

QVector<VendaRecord> VendaDAO::listByPeriod(const QDateTime &inicio, const QDateTime &fim)
{
    QVector<VendaRecord> vendas;

    if (!m_databaseManager.open()) {
        setLastError(m_databaseManager.lastError());
        return vendas;
    }

    QSqlQuery query(m_databaseManager.database());
    query.prepare(QStringLiteral(
        "SELECT id, funcionario_id, data_venda, valor_total FROM vendas "
        "WHERE data_venda BETWEEN :inicio AND :fim ORDER BY data_venda"));
    query.bindValue(QStringLiteral(":inicio"), dateTimeToDatabase(inicio));
    query.bindValue(QStringLiteral(":fim"), dateTimeToDatabase(fim));

    if (!query.exec()) {
        setLastError(query.lastError().text());
        return vendas;
    }

    while (query.next()) {
        vendas.append(mapVenda(query));
    }

    m_lastError.clear();
    return vendas;
}

QString VendaDAO::lastError() const
{
    return m_lastError;
}

VendaRecord VendaDAO::mapVenda(const QSqlQuery &query) const
{
    VendaRecord venda;
    venda.id = query.value(0).toInt();
    venda.funcionarioId = query.value(1).toInt();
    venda.dataVenda = parseDateTime(query.value(2));
    venda.valorTotal = query.value(3).toDouble();
    return venda;
}

void VendaDAO::setLastError(const QString &message)
{
    m_lastError = message;
}
