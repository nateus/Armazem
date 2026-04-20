#ifndef USUARIODAO_H
#define USUARIODAO_H

#include "Models/Usuario.h"

#include <QVector>
#include <QString>

#include <optional>

class DatabaseManager;
class QSqlQuery;

class UsuarioDAO
{
public:
    explicit UsuarioDAO(DatabaseManager &databaseManager);

    bool existsAdmin();
    std::optional<Usuario> findByLogin(const QString &login);
    int insert(const Usuario &usuario);
    bool update(const Usuario &usuario);
    bool setActive(int usuarioId, bool ativo);
    QVector<Usuario> list(bool includeInactive = true);

    QString lastError() const;

private:
    Usuario mapUsuario(const QSqlQuery &query) const;
    void setLastError(const QString &message);

    DatabaseManager &m_databaseManager;
    QString m_lastError;
};

#endif // USUARIODAO_H
