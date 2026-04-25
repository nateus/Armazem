#include "cadastroadminwindow.h"

#include "ui_cadastroadminwindow.h"

#include "../Services/AuthService.h"

#include <QLineEdit>
#include <QMessageBox>

CadastroAdminWindow::CadastroAdminWindow(AuthService *authService, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CadastroAdminWindow)
    , m_authService(authService)
{
    ui->setupUi(this);
    ui->textEdit->setEchoMode(QLineEdit::Password);
    ui->textEdit_2->setEchoMode(QLineEdit::Password);
}

CadastroAdminWindow::~CadastroAdminWindow()
{
    delete ui;
}

void CadastroAdminWindow::on_pushButton_clicked()
{
    const QString senha = ui->textEdit->text();
    const QString confirma = ui->textEdit_2->text();

    if (senha.isEmpty() || confirma.isEmpty()) {
        QMessageBox::warning(this, "Erro", "Preencha todos os campos!");
        return;
    }

    if (senha != confirma) {
        QMessageBox::warning(this, "Erro", "As senhas nao coincidem!");
        return;
    }

    if (!m_authService) {
        QMessageBox::critical(this, "Erro", "Servico de autenticacao indisponivel.");
        return;
    }

    if (m_authService->registerInitialAdmin(senha)) {
        ui->textEdit->clear();
        ui->textEdit_2->clear();
        QMessageBox::information(this, "Sucesso", "Administrador registrado! Faca o login.");
        emit adminRegistered();
    } else {
        QMessageBox::warning(
            this,
            "Erro",
            m_authService->lastError().isEmpty()
                ? QStringLiteral("Falha ao registrar administrador.")
                : m_authService->lastError());
    }
}
