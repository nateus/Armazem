#ifndef SCHEMAINITIALIZER_H
#define SCHEMAINITIALIZER_H

#include <QString>

class DatabaseManager;

class SchemaInitializer
{
public:
    explicit SchemaInitializer(DatabaseManager &databaseManager);

    bool initialize();
    QString lastError() const;

private:
    bool execute(const QString &sql);
    void setLastError(const QString &message);

    DatabaseManager &m_databaseManager;
    QString m_lastError;
};

#endif // SCHEMAINITIALIZER_H
