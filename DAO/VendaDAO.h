#ifndef VENDADAO_H
#define VENDADAO_H

#include <QDateTime>
#include <QVector>
#include <QString>

class DatabaseManager;
class QSqlQuery;

struct VendaRecord
{
    int id = 0;
    int funcionarioId = 0;
    QDateTime dataVenda;
    double valorTotal = 0.0;
};

struct ItemVendaRecord
{
    int id = 0;
    int vendaId = 0;
    int produtoId = 0;
    int quantidade = 0;
    double precoUnitario = 0.0;
    double subtotal = 0.0;
};

class VendaDAO
{
public:
    explicit VendaDAO(DatabaseManager &databaseManager);

    int insertVenda(const VendaRecord &venda);
    int insertItem(const ItemVendaRecord &item);
    QVector<VendaRecord> listByPeriod(const QDateTime &inicio, const QDateTime &fim);

    QString lastError() const;

private:
    VendaRecord mapVenda(const QSqlQuery &query) const;
    void setLastError(const QString &message);

    DatabaseManager &m_databaseManager;
    QString m_lastError;
};

#endif // VENDADAO_H
