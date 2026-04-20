#include "cadastroadminwindow.h"
#include "ui_cadastroadminwindow.h"
#include "../Services/AuthService.h"
#include <QMessageBox>

CadastroAdminWindow::CadastroAdminWindow(AuthService* authService, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CadastroAdminWindow)
    , m_authService(authService)
{
    ui->setupUi(this);
}

CadastroAdminWindow::~CadastroAdminWindow()
{
    delete ui;
}

void CadastroAdminWindow::on_pushButton_clicked()
{
    QString senha = ui->textEdit->text();
    QString confirma = ui->textEdit_2->text();

    if (senha.isEmpty() || confirma.isEmpty()) {
        QMessageBox::warning(this, "Erro", "Preencha todos os campos!");
        return;
    }

    if (senha != confirma) {
        QMessageBox::warning(this, "Erro", "As senhas não coincidem!");
        return;
    }

    if (m_authService->registerInitialAdmin(senha)) {
        QMessageBox::information(this, "Sucesso", "Administrador registrado! Faça o login.");
        emit adminRegistered();
    } else {
        QMessageBox::critical(this, "Erro", "Falha ao registrar administrador.");
    }
}
