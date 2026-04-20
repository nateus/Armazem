#include "cadastrousuariowindow.h"
#include "ui_cadastrousuariowindow.h"
#include "../Services/UserService.h"
#include <QMessageBox>

CadastroUsuarioWindow::CadastroUsuarioWindow(UserService* userService, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CadastroUsuarioWindow)
    , m_userService(userService)
{
    ui->setupUi(this);

    // Popular o ComboBox com os tipos de usuário permitidos (regras de negócio)
    ui->comboBox->addItem("gerente");
    ui->comboBox->addItem("funcionario");
    
    // Por padrão, ativo vem marcado
    ui->checkBox->setChecked(true); 
}

CadastroUsuarioWindow::~CadastroUsuarioWindow()
{
    delete ui;
}

void CadastroUsuarioWindow::on_pushButton_clicked()
{
    // Usando toPlainText() pois estamos usando QTextEdit
    QString nome = ui->textEdit->toPlainText().trimmed();
    QString login = ui->textEdit_2->toPlainText().trimmed();
    QString senha = ui->textEdit_3->toPlainText();
    QString tipo = ui->comboBox->currentText();
    bool ativo = ui->checkBox->isChecked();

    QString errorMessage;
    if (m_userService->cadastrarUsuario(nome, login, senha, tipo, ativo, errorMessage)) {
        QMessageBox::information(this, "Sucesso", "Usuário cadastrado com sucesso!");
        emit windowClosed();
        this->close();
    } else {
        QMessageBox::warning(this, "Erro", errorMessage);
    }
}

void CadastroUsuarioWindow::on_pushButton_2_clicked()
{
    emit windowClosed();
    this->close();
}
