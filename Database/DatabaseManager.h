#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QString>

class DatabaseManager
{
public:
    explicit DatabaseManager(const QString &databasePath,
                             const QString &connectionName = QStringLiteral("armazem_connection"));
    ~DatabaseManager();

    bool open();
    bool isOpen() const;
    void close();

    QSqlDatabase database() const;
    QString lastError() const;

    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();

private:
    bool ensureDatabaseHandle();
    void setLastError(const QString &message);

    QString m_databasePath;
    QString m_connectionName;
    QString m_lastError;
};

#endif // DATABASEMANAGER_H
