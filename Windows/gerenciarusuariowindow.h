#ifndef GERENCIARUSUARIOWINDOW_H
#define GERENCIARUSUARIOWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

class UserService; // Forward declaration

namespace Ui {
class GerenciarUsuarioWindow;
}

class GerenciarUsuarioWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GerenciarUsuarioWindow(UserService* userService, QWidget *parent = nullptr);
    ~GerenciarUsuarioWindow();

private slots:
    void carregarDados(); // Atualiza a tabela
    void on_pushButton_3_clicked(); // Editar
    void on_pushButton_2_clicked(); // Ativar/Desativar
    void on_pushButton_clicked();   // Redefinir Senha
    void on_comboBox_currentTextChanged(const QString &arg1); // Filtrar
    void on_textEdit_textChanged(); // Buscar

private:
    Ui::GerenciarUsuarioWindow *ui;
    UserService* m_userService;
    QStandardItemModel* m_tableModel;

    int getSelectedUserId(); // Método auxiliar para pegar o ID selecionado
};

#endif // GERENCIARUSUARIOWINDOW_H
