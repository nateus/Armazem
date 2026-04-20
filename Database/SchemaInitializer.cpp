#include "Database/SchemaInitializer.h"

#include "Database/DatabaseManager.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>

SchemaInitializer::SchemaInitializer(DatabaseManager &databaseManager)
    : m_databaseManager(databaseManager)
{
}

bool SchemaInitializer::initialize()
{
    if (!m_databaseManager.open()) {
        setLastError(m_databaseManager.lastError());
        return false;
    }

    const QStringList statements = {
        QStringLiteral("PRAGMA foreign_keys = ON"),
        QStringLiteral(
            "CREATE TABLE IF NOT EXISTS usuarios ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "nome TEXT NOT NULL,"
            "login TEXT NOT NULL UNIQUE,"
            "senha_hash TEXT NOT NULL,"
            "tipo TEXT NOT NULL CHECK (tipo IN ('admin', 'gerente', 'funcionario')),"
            "ativo INTEGER NOT NULL DEFAULT 1,"
            "data_criacao TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP"
            ")"),
        QStringLiteral("CREATE INDEX IF NOT EXISTS idx_usuarios_login ON usuarios(login)"),
        QStringLiteral("CREATE INDEX IF NOT EXISTS idx_usuarios_tipo ON usuarios(tipo)"),
        QStringLiteral(
            "CREATE TABLE IF NOT EXISTS categorias ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "nome TEXT NOT NULL UNIQUE,"
            "descricao TEXT"
            ")"),
        QStringLiteral("CREATE INDEX IF NOT EXISTS idx_categorias_nome ON categorias(nome)"),
        QStringLiteral(
            "CREATE TABLE IF NOT EXISTS produtos ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "codigo TEXT NOT NULL UNIQUE,"
            "nome TEXT NOT NULL,"
            "descricao TEXT,"
            "preco REAL NOT NULL CHECK (preco >= 0),"
            "quantidade INTEGER NOT NULL DEFAULT 0 CHECK (quantidade >= 0),"
            "categoria_id INTEGER,"
            "ativo INTEGER NOT NULL DEFAULT 1,"
            "data_cadastro TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,"
            "FOREIGN KEY (categoria_id) REFERENCES categorias(id)"
            ")"),
        QStringLiteral("CREATE INDEX IF NOT EXISTS idx_produtos_codigo ON produtos(codigo)"),
        QStringLiteral("CREATE INDEX IF NOT EXISTS idx_produtos_nome ON produtos(nome)"),
        QStringLiteral("CREATE INDEX IF NOT EXISTS idx_produtos_ativo ON produtos(ativo)"),
        QStringLiteral("CREATE INDEX IF NOT EXISTS idx_produtos_categoria ON produtos(categoria_id)"),
        QStringLiteral(
            "CREATE TABLE IF NOT EXISTS vendas ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "funcionario_id INTEGER NOT NULL,"
            "data_venda TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,"
            "valor_total REAL NOT NULL CHECK (valor_total >= 0),"
            "FOREIGN KEY (funcionario_id) REFERENCES usuarios(id)"
            ")"),
        QStringLiteral("CREATE INDEX IF NOT EXISTS idx_vendas_funcionario ON vendas(funcionario_id)"),
        QStringLiteral("CREATE INDEX IF NOT EXISTS idx_vendas_data ON vendas(data_venda)"),
        QStringLiteral(
            "CREATE TABLE IF NOT EXISTS itens_venda ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "venda_id INTEGER NOT NULL,"
            "produto_id INTEGER NOT NULL,"
            "quantidade INTEGER NOT NULL CHECK (quantidade > 0),"
            "preco_unitario REAL NOT NULL CHECK (preco_unitario >= 0),"
            "subtotal REAL NOT NULL CHECK (subtotal >= 0),"
            "FOREIGN KEY (venda_id) REFERENCES vendas(id) ON DELETE CASCADE,"
            "FOREIGN KEY (produto_id) REFERENCES produtos(id)"
            ")"),
        QStringLiteral("CREATE INDEX IF NOT EXISTS idx_itens_venda_venda ON itens_venda(venda_id)"),
        QStringLiteral("CREATE INDEX IF NOT EXISTS idx_itens_venda_produto ON itens_venda(produto_id)"),
        QStringLiteral(
            "CREATE TABLE IF NOT EXISTS movimentacoes_estoque ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "produto_id INTEGER NOT NULL,"
            "usuario_id INTEGER,"
            "tipo TEXT NOT NULL CHECK (tipo IN ('entrada', 'saida', 'ajuste')),"
            "quantidade INTEGER NOT NULL CHECK (quantidade > 0),"
            "motivo TEXT,"
            "data_movimentacao TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,"
            "FOREIGN KEY (produto_id) REFERENCES produtos(id),"
            "FOREIGN KEY (usuario_id) REFERENCES usuarios(id)"
            ")"),
        QStringLiteral("CREATE INDEX IF NOT EXISTS idx_movimentacoes_produto ON movimentacoes_estoque(produto_id)"),
        QStringLiteral("CREATE INDEX IF NOT EXISTS idx_movimentacoes_usuario ON movimentacoes_estoque(usuario_id)"),
        QStringLiteral("CREATE INDEX IF NOT EXISTS idx_movimentacoes_data ON movimentacoes_estoque(data_movimentacao)"),
        QStringLiteral(
            "CREATE TABLE IF NOT EXISTS logs_sistema ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "usuario_id INTEGER,"
            "acao TEXT NOT NULL,"
            "descricao TEXT,"
            "data_log TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,"
            "FOREIGN KEY (usuario_id) REFERENCES usuarios(id)"
            ")"),
        QStringLiteral("CREATE INDEX IF NOT EXISTS idx_logs_usuario ON logs_sistema(usuario_id)"),
        QStringLiteral("CREATE INDEX IF NOT EXISTS idx_logs_data ON logs_sistema(data_log)")
    };

    for (const QString &statement : statements) {
        if (!execute(statement)) {
            return false;
        }
    }

    m_lastError.clear();
    return true;
}

QString SchemaInitializer::lastError() const
{
    return m_lastError;
}

bool SchemaInitializer::execute(const QString &sql)
{
    QSqlQuery query(m_databaseManager.database());
    if (!query.exec(sql)) {
        setLastError(query.lastError().text());
        return false;
    }

    return true;
}

void SchemaInitializer::setLastError(const QString &message)
{
    m_lastError = message;
}
