#include "movimentacaoestoquewindow.h"
#include "ui_movimentacaoestoquewindow.h"
#include "../Services/InventoryService.h"
#include <QMessageBox>

MovimentacaoEstoqueWindow::MovimentacaoEstoqueWindow(InventoryService* inventoryService, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MovimentacaoEstoqueWindow)
    , m_inventoryService(inventoryService)
    , m_tableModel(new QStandardItemModel(this))
{
    ui->setupUi(this);

    // Configurando os Tipos de Movimentação do Banco de Dados
    ui->comboBox->addItem("entrada");
    ui->comboBox->addItem("saida");
    ui->comboBox->addItem("ajuste");

    // Configurando a Tabela
    m_tableModel->setHorizontalHeaderLabels({"ID", "Código", "Nome do Produto", "Estoque Atual"});
    ui->tableView->setModel(m_tableModel);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // Somente leitura
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    carregarProdutos();
}

MovimentacaoEstoqueWindow::~MovimentacaoEstoqueWindow()
{
    delete ui;
}

void MovimentacaoEstoqueWindow::carregarProdutos()
{
    m_tableModel->removeRows(0, m_tableModel->rowCount());

    QString busca = ui->textEdit->toPlainText().trimmed();
    std::vector<ProdutoEstoque> produtos = m_inventoryService->listarProdutosParaEstoque(busca);

    for (const auto& p : produtos) {
        QList<QStandardItem*> row;
        row.append(new QStandardItem(QString::number(p.id)));
        row.append(new QStandardItem(p.codigo));
        row.append(new QStandardItem(p.nome));
        
        // Destaque visual na quantidade
        QStandardItem* itemQtd = new QStandardItem(QString::number(p.quantidadeAtual));
        itemQtd->setTextAlignment(Qt::AlignCenter);
        row.append(itemQtd);
        
        m_tableModel->appendRow(row);
    }
}

int MovimentacaoEstoqueWindow::getSelectedProductId()
{
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) return -1;
    return m_tableModel->item(selectedRows.first().row(), 0)->text().toInt();
}

void MovimentacaoEstoqueWindow::limparFormulario()
{
    ui->spinBox->setValue(0);
    ui->textEdit_2->clear(); // Limpa o motivo
    ui->comboBox->setCurrentIndex(0);
    // Remove a seleção da tabela para forçar o usuário a escolher novamente
    ui->tableView->clearSelection(); 
}

void MovimentacaoEstoqueWindow::on_textEdit_textChanged()
{
    carregarProdutos(); // Busca em tempo real
}

void MovimentacaoEstoqueWindow::on_pushButton_clicked() // Confirmar
{
    int produtoId = getSelectedProductId();
    if (produtoId == -1) {
        QMessageBox::warning(this, "Atenção", "Selecione um produto na tabela.");
        return;
    }

    QString tipo = ui->comboBox->currentText();
    int quantidade = ui->spinBox->value();
    QString motivo = ui->textEdit_2->toPlainText().trimmed();

    QString errorMessage;
    if (m_inventoryService->registrarMovimentacao(produtoId, tipo, quantidade, motivo, errorMessage)) {
        QMessageBox::information(this, "Sucesso", "Movimentação registrada com sucesso!");
        limparFormulario();
        carregarProdutos(); // Atualiza a tabela com o novo estoque
    } else {
        QMessageBox::critical(this, "Erro de Validação", errorMessage);
    }
}
