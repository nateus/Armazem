#include "DAO/MovimentacaoDAO.h"

#include "Database/DatabaseManager.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

namespace {

QString tipoMovimentacaoToDatabase(TipoMovimentacao tipo)
{
    switch (tipo) {
    case TipoMovimentacao::Entrada:
        return QStringLiteral("entrada");
    case TipoMovimentacao::Saida:
        return QStringLiteral("saida");
    case TipoMovimentacao::Ajuste:
        return QStringLiteral("ajuste");
    case TipoMovimentacao::Invalido:
        break;
    }

    return QStringLiteral("ajuste");
}

TipoMovimentacao tipoMovimentacaoFromDatabase(const QString &tipo)
{
    const QString normalized = tipo.toLower();
    if (normalized == QStringLiteral("entrada")) {
        return TipoMovimentacao::Entrada;
    }
    if (normalized == QStringLiteral("saida")) {
        return TipoMovimentacao::Saida;
    }
    if (normalized == QStringLiteral("ajuste")) {
        return TipoMovimentacao::Ajuste;
    }

    return TipoMovimentacao::Invalido;
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

MovimentacaoDAO::MovimentacaoDAO(DatabaseManager &databaseManager)
    : m_databaseManager(databaseManager)
{
}

int MovimentacaoDAO::insert(const MovimentacaoRecord &movimentacao)
{
    if (!m_databaseManager.open()) {
        setLastError(m_databaseManager.lastError());
        return 0;
    }

    QSqlQuery query(m_databaseManager.database());
    query.prepare(QStringLiteral(
        "INSERT INTO movimentacoes_estoque "
        "(produto_id, usuario_id, tipo, quantidade, motivo, data_movimentacao) "
        "VALUES (:produto_id, :usuario_id, :tipo, :quantidade, :motivo, :data_movimentacao)"));
    query.bindValue(QStringLiteral(":produto_id"), movimentacao.produtoId);
    if (movimentacao.usuarioId > 0) {
        query.bindValue(QStringLiteral(":usuario_id"), movimentacao.usuarioId);
    } else {
        query.bindValue(QStringLiteral(":usuario_id"), QVariant());
    }
    query.bindValue(QStringLiteral(":tipo"), tipoMovimentacaoToDatabase(movimentacao.tipo));
    query.bindValue(QStringLiteral(":quantidade"), movimentacao.quantidade);
    query.bindValue(QStringLiteral(":motivo"), movimentacao.motivo);
    query.bindValue(QStringLiteral(":data_movimentacao"), dateTimeToDatabase(movimentacao.dataMovimentacao));

    if (!query.exec()) {
        setLastError(query.lastError().text());
        return 0;
    }

    m_lastError.clear();
    return query.lastInsertId().toInt();
}

QVector<MovimentacaoRecord> MovimentacaoDAO::listByPeriod(const QDateTime &inicio, const QDateTime &fim)
{
    QVector<MovimentacaoRecord> movimentacoes;

    if (!m_databaseManager.open()) {
        setLastError(m_databaseManager.lastError());
        return movimentacoes;
    }

    QSqlQuery query(m_databaseManager.database());
    query.prepare(QStringLiteral(
        "SELECT id, produto_id, usuario_id, tipo, quantidade, motivo, data_movimentacao "
        "FROM movimentacoes_estoque WHERE data_movimentacao BETWEEN :inicio AND :fim "
        "ORDER BY data_movimentacao"));
    query.bindValue(QStringLiteral(":inicio"), dateTimeToDatabase(inicio));
    query.bindValue(QStringLiteral(":fim"), dateTimeToDatabase(fim));

    if (!query.exec()) {
        setLastError(query.lastError().text());
        return movimentacoes;
    }

    while (query.next()) {
        movimentacoes.append(mapMovimentacao(query));
    }

    m_lastError.clear();
    return movimentacoes;
}

QVector<MovimentacaoRecord> MovimentacaoDAO::listByProduct(int produtoId)
{
    QVector<MovimentacaoRecord> movimentacoes;

    if (!m_databaseManager.open()) {
        setLastError(m_databaseManager.lastError());
        return movimentacoes;
    }

    QSqlQuery query(m_databaseManager.database());
    query.prepare(QStringLiteral(
        "SELECT id, produto_id, usuario_id, tipo, quantidade, motivo, data_movimentacao "
        "FROM movimentacoes_estoque WHERE produto_id = :produto_id ORDER BY data_movimentacao"));
    query.bindValue(QStringLiteral(":produto_id"), produtoId);

    if (!query.exec()) {
        setLastError(query.lastError().text());
        return movimentacoes;
    }

    while (query.next()) {
        movimentacoes.append(mapMovimentacao(query));
    }

    m_lastError.clear();
    return movimentacoes;
}

QString MovimentacaoDAO::lastError() const
{
    return m_lastError;
}

MovimentacaoRecord MovimentacaoDAO::mapMovimentacao(const QSqlQuery &query) const
{
    MovimentacaoRecord movimentacao;
    movimentacao.id = query.value(0).toInt();
    movimentacao.produtoId = query.value(1).toInt();
    movimentacao.usuarioId = query.value(2).toInt();
    movimentacao.tipo = tipoMovimentacaoFromDatabase(query.value(3).toString());
    movimentacao.quantidade = query.value(4).toInt();
    movimentacao.motivo = query.value(5).toString();
    movimentacao.dataMovimentacao = parseDateTime(query.value(6));
    return movimentacao;
}

void MovimentacaoDAO::setLastError(const QString &message)
{
    m_lastError = message;
}
