#ifndef LOGDAO_H
#define LOGDAO_H

#include <QDateTime>
#include <QVector>
#include <QString>

class DatabaseManager;
class QSqlQuery;

struct LogRecord
{
    int id = 0;
    int usuarioId = 0;
    QString acao;
    QString descricao;
    QDateTime dataLog;
};

class LogDAO
{
public:
    explicit LogDAO(DatabaseManager &databaseManager);

    int insert(const LogRecord &log);
    QVector<LogRecord> listByPeriod(const QDateTime &inicio, const QDateTime &fim);

    QString lastError() const;

private:
    LogRecord mapLog(const QSqlQuery &query) const;
    void setLastError(const QString &message);

    DatabaseManager &m_databaseManager;
    QString m_lastError;
};

#endif // LOGDAO_H
