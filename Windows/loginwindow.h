#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
class AuthService;

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(AuthService* authService, QWidget *parent = nullptr);
    ~LoginWindow();

signals:
    void loginSuccessful(); // Sinaliza que a sessão começou com sucesso

private slots:
    void on_pushButton_clicked(); // Botão "Login"
    void on_pushButton_2_clicked(); // Botão "Sair"
    void on_checkBox_toggled(bool checked); // "Mostrar Senha"

private:
    Ui::LoginWindow *ui;
    AuthService* m_authService;
};

#endif // LOGINWINDOW_H
