#include "loginwindow.h"

#include "ui_loginwindow.h"

#include "../Services/AuthService.h"

#include <QApplication>
#include <QLineEdit>
#include <QMessageBox>

LoginWindow::LoginWindow(AuthService *authService, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWindow)
    , m_authService(authService)
{
    ui->setupUi(this);
    ui->textEdit_2->setEchoMode(QLineEdit::Password);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_pushButton_clicked()
{
    const QString usuario = ui->textEdit->text().trimmed();
    const QString senha = ui->textEdit_2->text();
    QString errorMsg;

    if (usuario.isEmpty() || senha.isEmpty()) {
        QMessageBox::warning(this, "Aviso", "Preencha usuario e senha.");
        return;
    }

    if (!m_authService) {
        QMessageBox::critical(this, "Erro", "Servico de autenticacao indisponivel.");
        return;
    }

    if (m_authService->login(usuario, senha, errorMsg)) {
        ui->textEdit_2->clear();
        QMessageBox::information(this, "Sucesso", "Login realizado com sucesso.");
        emit loginSuccessful();
    } else {
        ui->textEdit_2->clear();
        QMessageBox::warning(this, "Acesso negado", errorMsg);
    }
}

void LoginWindow::on_pushButton_2_clicked()
{
    QApplication::quit();
}

void LoginWindow::on_checkBox_toggled(bool checked)
{
    ui->textEdit_2->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
}
