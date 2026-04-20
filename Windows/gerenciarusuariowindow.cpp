#include "gerenciarusuariowindow.h"
#include "ui_gerenciarusuariowindow.h"
#include "../Services/UserService.h"
#include <QMessageBox>
#include <QInputDialog>

GerenciarUsuarioWindow::GerenciarUsuarioWindow(UserService* userService, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GerenciarUsuarioWindow)
    , m_userService(userService)
    , m_tableModel(new QStandardItemModel(this))
{
    ui->setupUi(this);

    // Configurar ComboBox de filtros
    ui->comboBox->addItem("Todos");
    ui->comboBox->addItem("admin");
    ui->comboBox->addItem("gerente");
    ui->comboBox->addItem("funcionario");

    // Configurar colunas da Tabela
    m_tableModel->setHorizontalHeaderLabels({"ID", "Nome", "Login", "Tipo", "Ativo"});
    ui->tableView->setModel(m_tableModel);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // Tabela read-only

    carregarDados();
}

GerenciarUsuarioWindow::~GerenciarUsuarioWindow()
{
    delete ui;
}

void GerenciarUsuarioWindow::carregarDados()
{
    m_tableModel->removeRows(0, m_tableModel->rowCount()); // Limpa a tabela

    QString filtro = ui->comboBox->currentText() == "Todos" ? "" : ui->comboBox->currentText();
    QString busca = ui->textEdit->toPlainText().trimmed(); // QTextEdit

    std::vector<Usuario> usuarios = m_userService->listarUsuarios(filtro, busca);

    for (const auto& u : usuarios) {
        QList<QStandardItem*> row;
        row.append(new QStandardItem(QString::number(u.id)));
        row.append(new QStandardItem(u.nome));
        row.append(new QStandardItem(u.login));
        row.append(new QStandardItem(u.tipo));
        row.append(new QStandardItem(u.ativo ? "Sim" : "Não"));
        m_tableModel->appendRow(row);
    }
}

int GerenciarUsuarioWindow::getSelectedUserId()
{
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) return -1;
    
    // A coluna 0 é o ID
    return m_tableModel->item(selectedRows.first().row(), 0)->text().toInt();
}

void GerenciarUsuarioWindow::on_textEdit_textChanged() { carregarDados(); }
void GerenciarUsuarioWindow::on_comboBox_currentTextChanged(const QString&) { carregarDados(); }

void GerenciarUsuarioWindow::on_pushButton_3_clicked() // Editar
{
    int id = getSelectedUserId();
    if (id == -1) {
        QMessageBox::warning(this, "Aviso", "Selecione um usuário para editar.");
        return;
    }
    QMessageBox::information(this, "Aviso", "Abre janela/dialog de edição preenchida com ID: " + QString::number(id));
    // Aqui você conectaria a um QDialog para editar, ou reusaria o CadastroUsuarioWindow.
}

void GerenciarUsuarioWindow::on_pushButton_2_clicked() // Ativar / Desativar
{
    int id = getSelectedUserId();
    if (id == -1) {
        QMessageBox::warning(this, "Aviso", "Selecione um usuário.");
        return;
    }

    // Pega o status atual na coluna 4 (Ativo)
    int row = ui->tableView->selectionModel()->selectedRows().first().row();
    bool statusAtual = (m_tableModel->item(row, 4)->text() == "Sim");

    QString errorMessage;
    if (m_userService->ativarDesativarUsuario(id, !statusAtual, errorMessage)) {
        carregarDados(); // Recarrega a tabela atualizada
    } else {
        QMessageBox::critical(this, "Erro", errorMessage);
    }
}

void GerenciarUsuarioWindow::on_pushButton_clicked() // Redefinir Senha
{
    int id = getSelectedUserId();
    if (id == -1) {
        QMessageBox::warning(this, "Aviso", "Selecione um usuário.");
        return;
    }

    bool ok;
    QString novaSenha = QInputDialog::getText(this, "Redefinir Senha",
                                              "Digite a nova senha:", QLineEdit::Password,
                                              "", &ok);
    if (ok && !novaSenha.isEmpty()) {
        QString errorMessage;
        if (m_userService->redefinirSenha(id, novaSenha, errorMessage)) {
            QMessageBox::information(this, "Sucesso", "Senha redefinida com sucesso!");
        } else {
            QMessageBox::critical(this, "Erro", errorMessage);
        }
    }
}
