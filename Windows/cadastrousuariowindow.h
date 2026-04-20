#ifndef CADASTROUSUARIOWINDOW_H
#define CADASTROUSUARIOWINDOW_H

#include <QWidget>
class UserService; // Forward declaration

namespace Ui {
class CadastroUsuarioWindow;
}

class CadastroUsuarioWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CadastroUsuarioWindow(UserService* userService, QWidget *parent = nullptr);
    ~CadastroUsuarioWindow();

signals:
    void windowClosed(); // Para avisar o Dashboard/WindowRouter

private slots:
    void on_pushButton_clicked();   // Salvar
    void on_pushButton_2_clicked(); // Cancelar

private:
    Ui::CadastroUsuarioWindow *ui;
    UserService* m_userService;
};

#endif // CADASTROUSUARIOWINDOW_H
