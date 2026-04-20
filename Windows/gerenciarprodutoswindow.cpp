#include "gerenciarprodutoswindow.h"
#include "ui_gerenciarprodutoswindow.h"
#include "../Services/ProductService.h"
#include <QMessageBox>

GerenciarProdutosWindow::GerenciarProdutosWindow(ProductService* productService, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GerenciarProdutosWindow)
    , m_productService(productService)
    , m_tableModel(new QStandardItemModel(this))
{
    ui->setupUi(this);

    // Configurando Tabela
    m_tableModel->setHorizontalHeaderLabels({"ID", "Código", "Nome", "Categoria", "Preço (R$)", "Estoque", "Ativo"});
    ui->tableView->setModel(m_tableModel);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    carregarCategoriasFiltro();
    carregarDados();
}

GerenciarProdutosWindow::~GerenciarProdutosWindow()
{
    delete ui;
}

void GerenciarProdutosWindow::carregarCategoriasFiltro()
{
    ui->comboBox->clear();
    ui->comboBox->addItem("Todas as Categorias", -1);

    std::vector<Categoria> categorias = m_productService->listarCategorias();
    for (const auto& cat : categorias) {
        ui->comboBox->addItem(cat.nome, cat.id);
    }
}

void GerenciarProdutosWindow::carregarDados()
{
    m_tableModel->removeRows(0, m_tableModel->rowCount());

    QString busca = ui->textEdit->toPlainText().trimmed();
    int categoriaId = ui->comboBox->currentData().toInt();

    std::vector<Produto> produtos = m_productService->listarProdutos(busca, categoriaId);

    for (const auto& p : produtos) {
        QList<QStandardItem*> row;
        row.append(new QStandardItem(QString::number(p.id)));
        row.append(new QStandardItem(p.codigo));
        row.append(new QStandardItem(p.nome));
        row.append(new QStandardItem(p.categoriaNome));
        row.append(new QStandardItem(QString::number(p.preco, 'f', 2)));
        row.append(new QStandardItem(QString::number(p.quantidade)));
        row.append(new QStandardItem(p.ativo ? "Sim" : "Não"));
        m_tableModel->appendRow(row);
    }
}

int GerenciarProdutosWindow::getSelectedProductId()
{
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) return -1;
    return m_tableModel->item(selectedRows.first().row(), 0)->text().toInt();
}

// Conectando os eventos
void GerenciarProdutosWindow::on_textEdit_textChanged() { carregarDados(); }
void GerenciarProdutosWindow::on_comboBox_currentIndexChanged(int) { carregarDados(); }
void GerenciarProdutosWindow::on_pushButton_4_clicked() { carregarDados(); }

void GerenciarProdutosWindow::on_pushButton_2_clicked() // Desativar
{
    int id = getSelectedProductId();
    if (id == -1) {
        QMessageBox::warning(this, "Aviso", "Selecione um produto.");
        return;
    }

    QString errorMessage;
    if (m_productService->desativarProduto(id, errorMessage)) {
        QMessageBox::information(this, "Sucesso", "Produto desativado com sucesso!");
        carregarDados();
    } else {
        QMessageBox::critical(this, "Erro", errorMessage);
    }
}
