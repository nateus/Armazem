#include "Database/DatabaseManager.h"

#include <QSqlError>
#include <QSqlQuery>

DatabaseManager::DatabaseManager(const QString &databasePath, const QString &connectionName)
    : m_databasePath(databasePath)
    , m_connectionName(connectionName)
{
}

DatabaseManager::~DatabaseManager()
{
    close();
}

bool DatabaseManager::open()
{
    if (!ensureDatabaseHandle()) {
        return false;
    }

    QSqlDatabase db = database();
    if (db.isOpen()) {
        return true;
    }

    db.setDatabaseName(m_databasePath);
    if (!db.open()) {
        setLastError(db.lastError().text());
        return false;
    }

    QSqlQuery query(db);
    if (!query.exec(QStringLiteral("PRAGMA foreign_keys = ON"))) {
        setLastError(query.lastError().text());
        return false;
    }

    m_lastError.clear();
    return true;
}

bool DatabaseManager::isOpen() const
{
    if (!QSqlDatabase::contains(m_connectionName)) {
        return false;
    }

    return database().isOpen();
}

void DatabaseManager::close()
{
    if (!QSqlDatabase::contains(m_connectionName)) {
        return;
    }

    {
        QSqlDatabase db = database();
        if (db.isOpen()) {
            db.close();
        }
    }

    QSqlDatabase::removeDatabase(m_connectionName);
}

QSqlDatabase DatabaseManager::database() const
{
    return QSqlDatabase::database(m_connectionName, false);
}

QString DatabaseManager::lastError() const
{
    return m_lastError;
}

bool DatabaseManager::beginTransaction()
{
    if (!isOpen() && !open()) {
        return false;
    }

    QSqlDatabase db = database();
    if (!db.transaction()) {
        setLastError(db.lastError().text());
        return false;
    }

    m_lastError.clear();
    return true;
}

bool DatabaseManager::commitTransaction()
{
    QSqlDatabase db = database();
    if (!db.commit()) {
        setLastError(db.lastError().text());
        return false;
    }

    m_lastError.clear();
    return true;
}

bool DatabaseManager::rollbackTransaction()
{
    QSqlDatabase db = database();
    if (!db.rollback()) {
        setLastError(db.lastError().text());
        return false;
    }

    m_lastError.clear();
    return true;
}

bool DatabaseManager::ensureDatabaseHandle()
{
    if (m_databasePath.trimmed().isEmpty()) {
        setLastError(QStringLiteral("Database path is empty."));
        return false;
    }

    if (QSqlDatabase::contains(m_connectionName)) {
        return true;
    }

    if (!QSqlDatabase::isDriverAvailable(QStringLiteral("QSQLITE"))) {
        setLastError(QStringLiteral("SQLite driver QSQLITE is not available."));
        return false;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), m_connectionName);
    db.setDatabaseName(m_databasePath);
    return true;
}

void DatabaseManager::setLastError(const QString &message)
{
    m_lastError = message;
}
