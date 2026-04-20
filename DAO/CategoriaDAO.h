#ifndef CATEGORIADAO_H
#define CATEGORIADAO_H

#include <QVector>
#include <QString>

#include <optional>

class DatabaseManager;
class QSqlQuery;

struct CategoriaRecord
{
    int id = 0;
    QString nome;
    QString descricao;
};

class CategoriaDAO
{
public:
    explicit CategoriaDAO(DatabaseManager &databaseManager);

    QVector<CategoriaRecord> listAll();
    std::optional<CategoriaRecord> findByName(const QString &nome);
    int createIfMissing(const QString &nome, const QString &descricao = QString());

    QString lastError() const;

private:
    CategoriaRecord mapCategoria(const QSqlQuery &query) const;
    void setLastError(const QString &message);

    DatabaseManager &m_databaseManager;
    QString m_lastError;
};

#endif // CATEGORIADAO_H
