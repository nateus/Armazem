#ifndef MOVIMENTACAODAO_H
#define MOVIMENTACAODAO_H

#include "Models/Enums.h"

#include <QDateTime>
#include <QVector>
#include <QString>

class DatabaseManager;
class QSqlQuery;

struct MovimentacaoRecord
{
    int id = 0;
    int produtoId = 0;
    int usuarioId = 0;
    TipoMovimentacao tipo = TipoMovimentacao::Invalido;
    int quantidade = 0;
    QString motivo;
    QDateTime dataMovimentacao;
};

class MovimentacaoDAO
{
public:
    explicit MovimentacaoDAO(DatabaseManager &databaseManager);

    int insert(const MovimentacaoRecord &movimentacao);
    QVector<MovimentacaoRecord> listByPeriod(const QDateTime &inicio, const QDateTime &fim);
    QVector<MovimentacaoRecord> listByProduct(int produtoId);

    QString lastError() const;

private:
    MovimentacaoRecord mapMovimentacao(const QSqlQuery &query) const;
    void setLastError(const QString &message);

    DatabaseManager &m_databaseManager;
    QString m_lastError;
};

#endif // MOVIMENTACAODAO_H
