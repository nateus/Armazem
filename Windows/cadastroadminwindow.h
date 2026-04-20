#ifndef CADASTROADMINWINDOW_H
#define CADASTROADMINWINDOW_H

#include <QWidget>
class AuthService;

namespace Ui {
class CadastroAdminWindow;
}

class CadastroAdminWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CadastroAdminWindow(AuthService* authService, QWidget *parent = nullptr);
    ~CadastroAdminWindow();

signals:
    void adminRegistered();

    private slots:
        void on_pushButton_clicked();

private:
    Ui::CadastroAdminWindow *ui;
    AuthService* m_authService;
};

#endif // CADASTROADMINWINDOW_H
