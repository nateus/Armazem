#ifndef USERSERVICE_H
#define USERSERVICE_H

#include <QString>
#include <vector>

// Forward declarations dos pacotes 2 e 3
class UsuarioDAO;

// Placeholder do Modelo Usuario (Pacote 2) para o código compilar
struct Usuario {
    int id;
    QString nome;
    QString login;
    QString tipo;
    bool ativo;
};

class UserService {
public:
    explicit UserService(UsuarioDAO* usuarioDao);

    bool cadastrarUsuario(const QString& nome, const QString& login, const QString& senha, const QString& tipo, bool ativo, QString& errorMessage);
    bool editarUsuario(int id, const QString& nome, const QString& login, const QString& tipo, QString& errorMessage);
    bool ativarDesativarUsuario(int id, bool novoStatus, QString& errorMessage);
    bool redefinirSenha(int id, const QString& novaSenha, QString& errorMessage);
    
    std::vector<Usuario> listarUsuarios(const QString& filtroTipo = "", const QString& buscaTexto = "");

private:
    UsuarioDAO* m_usuarioDao;

    // Método auxiliar para verificar permissão do Pacote 4
    bool isAdminLogado() const; 
};

#endif // USERSERVICE_H
