#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "../Services/AuthService.h"
#include <QMessageBox>

LoginWindow::LoginWindow(AuthService* authService, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWindow)
    , m_authService(authService)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_pushButton_clicked()
{
    QString usuario = ui->textEdit->text(); 
    QString senha = ui->textEdit_2->text();
    QString errorMsg;

    if (usuario.isEmpty() || senha.isEmpty()) {
        QMessageBox::warning(this, "Aviso", "Preencha usuário e senha.");
        return;
    }

    if (m_authService->login(usuario, senha, errorMsg)) {
        emit loginSuccessful();
    } else {
        QMessageBox::critical(this, "Acesso Negado", errorMsg);
    }
}

void LoginWindow::on_pushButton_2_clicked()
{
    QApplication::quit();
}

void LoginWindow::on_checkBox_toggled(bool checked)
{

    /*if (checked) {
        ui->inputSenha->setEchoMode(QLineEdit::Normal);
    } else {
        ui->inputSenha->setEchoMode(QLineEdit::Password);
    }*/
}
