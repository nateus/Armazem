#include "DAO/LogDAO.h"

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

LogDAO::LogDAO(DatabaseManager &databaseManager)
    : m_databaseManager(databaseManager)
{
}

int LogDAO::insert(const LogRecord &log)
{
    if (!m_databaseManager.open()) {
        setLastError(m_databaseManager.lastError());
        return 0;
    }

    QSqlQuery query(m_databaseManager.database());
    query.prepare(QStringLiteral(
        "INSERT INTO logs_sistema (usuario_id, acao, descricao, data_log) "
        "VALUES (:usuario_id, :acao, :descricao, :data_log)"));
    if (log.usuarioId > 0) {
        query.bindValue(QStringLiteral(":usuario_id"), log.usuarioId);
    } else {
        query.bindValue(QStringLiteral(":usuario_id"), QVariant());
    }
    query.bindValue(QStringLiteral(":acao"), log.acao);
    query.bindValue(QStringLiteral(":descricao"), log.descricao);
    query.bindValue(QStringLiteral(":data_log"), dateTimeToDatabase(log.dataLog));

    if (!query.exec()) {
        setLastError(query.lastError().text());
        return 0;
    }

    m_lastError.clear();
    return query.lastInsertId().toInt();
}

QVector<LogRecord> LogDAO::listByPeriod(const QDateTime &inicio, const QDateTime &fim)
{
    QVector<LogRecord> logs;

    if (!m_databaseManager.open()) {
        setLastError(m_databaseManager.lastError());
        return logs;
    }

    QSqlQuery query(m_databaseManager.database());
    query.prepare(QStringLiteral(
        "SELECT id, usuario_id, acao, descricao, data_log FROM logs_sistema "
        "WHERE data_log BETWEEN :inicio AND :fim ORDER BY data_log"));
    query.bindValue(QStringLiteral(":inicio"), dateTimeToDatabase(inicio));
    query.bindValue(QStringLiteral(":fim"), dateTimeToDatabase(fim));

    if (!query.exec()) {
        setLastError(query.lastError().text());
        return logs;
    }

    while (query.next()) {
        logs.append(mapLog(query));
    }

    m_lastError.clear();
    return logs;
}

QString LogDAO::lastError() const
{
    return m_lastError;
}

LogRecord LogDAO::mapLog(const QSqlQuery &query) const
{
    LogRecord log;
    log.id = query.value(0).toInt();
    log.usuarioId = query.value(1).toInt();
    log.acao = query.value(2).toString();
    log.descricao = query.value(3).toString();
    log.dataLog = parseDateTime(query.value(4));
    return log;
}

void LogDAO::setLastError(const QString &message)
{
    m_lastError = message;
}
