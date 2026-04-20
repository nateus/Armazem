#ifndef PRODUTODAO_H
#define PRODUTODAO_H

#include <QDateTime>
#include <QVector>
#include <QString>

#include <optional>

class DatabaseManager;
class QSqlQuery;

struct ProdutoRecord
{
    int id = 0;
    QString codigo;
    QString nome;
    QString descricao;
    double preco = 0.0;
    int quantidade = 0;
    int categoriaId = 0;
    bool ativo = true;
    QDateTime dataCadastro;
};

class ProdutoDAO
{
public:
    explicit ProdutoDAO(DatabaseManager &databaseManager);

    int insert(const ProdutoRecord &produto);
    bool update(const ProdutoRecord &produto);
    bool setActive(int produtoId, bool ativo);
    std::optional<ProdutoRecord> findById(int produtoId);
    QVector<ProdutoRecord> search(const QString &filtro, bool onlyActive = false);

    QString lastError() const;

private:
    ProdutoRecord mapProduto(const QSqlQuery &query) const;
    void bindProduto(QSqlQuery &query, const ProdutoRecord &produto) const;
    void setLastError(const QString &message);

    DatabaseManager &m_databaseManager;
    QString m_lastError;
};

#endif // PRODUTODAO_H
