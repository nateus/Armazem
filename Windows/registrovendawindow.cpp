#include "registrovendawindow.h"
#include "ui_registrovendawindow.h"
#include <QMessageBox>

RegistroVendaWindow::RegistroVendaWindow(SalesService* salesService, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegistroVendaWindow)
    , m_salesService(salesService)
    , m_searchModel(new QStandardItemModel(this))
    , m_cartModel(new QStandardItemModel(this))
{
    ui->setupUi(this);

    // Configurando Tabela de Busca (tableView_2 - pequena)
    m_searchModel->setHorizontalHeaderLabels({"ID", "Nome", "Preço", "Estoque"});
    ui->tableView_2->setModel(m_searchModel);
    ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_2->setColumnHidden(0, true); // Oculta o ID

    // Configurando Tabela do Carrinho (tableView - grande)
    m_cartModel->setHorizontalHeaderLabels({"ID", "Produto", "Qtd", "Preço Un.", "Subtotal"});
    ui->tableView->setModel(m_cartModel);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setColumnHidden(0, true); // Oculta o ID

    // Quantidade mínima no spinbox
    ui->spinBox->setMinimum(1);
    
    // Carrega a busca inicial (vazia)
    on_textEdit_2_textChanged(); 
}

RegistroVendaWindow::~RegistroVendaWindow()
{
    delete ui;
}

void RegistroVendaWindow::on_textEdit_2_textChanged()
{
    m_searchModel->removeRows(0, m_searchModel->rowCount());
    QString termo = ui->textEdit_2->toPlainText().trimmed();
    
    auto produtos = m_salesService->buscarProdutosAtivos(termo);
    for (const auto& p : produtos) {
        QList<QStandardItem*> row;
        row.append(new QStandardItem(QString::number(p.id)));
        row.append(new QStandardItem(p.nome));
        row.append(new QStandardItem(QString::number(p.preco, 'f', 2)));
        row.append(new QStandardItem(QString::number(p.estoqueDisponivel)));
        m_searchModel->appendRow(row);
    }
}

void RegistroVendaWindow::on_pushButton_clicked() // Adicionar Item
{
    QModelIndexList selecao = ui->tableView_2->selectionModel()->selectedRows();
    if (selecao.isEmpty()) {
        QMessageBox::warning(this, "Aviso", "Selecione um produto na busca primeiro.");
        return;
    }

    int row = selecao.first().row();
    int id = m_searchModel->item(row, 0)->text().toInt();
    QString nome = m_searchModel->item(row, 1)->text();
    double preco = m_searchModel->item(row, 2)->text().toDouble();
    int estoque = m_searchModel->item(row, 3)->text().toInt();
    int qtdDesejada = ui->spinBox->value();

    if (qtdDesejada > estoque) {
        QMessageBox::warning(this, "Aviso", "Estoque insuficiente para esta quantidade.");
        return;
    }

    // Verifica se já existe no carrinho para somar a quantidade
    bool achou = false;
    for (auto& item : m_carrinho) {
        if (item.produtoId == id) {
            if (item.quantidade + qtdDesejada > estoque) {
                QMessageBox::warning(this, "Aviso", "A quantidade total no carrinho excede o estoque.");
                return;
            }
            item.quantidade += qtdDesejada;
            item.subtotal = item.quantidade * item.precoUnitario;
            achou = true;
            break;
        }
    }

    if (!achou) {
        ItemCarrinho novoItem = {id, "", nome, preco, qtdDesejada, preco * qtdDesejada};
        m_carrinho.push_back(novoItem);
    }

    atualizarTabelaCarrinho();
    ui->spinBox->setValue(1); // Reseta o contador
}

void RegistroVendaWindow::on_pushButton_3_clicked() // Remover Item
{
    QModelIndexList selecao = ui->tableView->selectionModel()->selectedRows();
    if (selecao.isEmpty()) {
        QMessageBox::warning(this, "Aviso", "Selecione um item no carrinho para remover.");
        return;
    }

    int row = selecao.first().row();
    m_carrinho.erase(m_carrinho.begin() + row);
    atualizarTabelaCarrinho();
}

void RegistroVendaWindow::atualizarTabelaCarrinho()
{
    m_cartModel->removeRows(0, m_cartModel->rowCount());
    for (const auto& item : m_carrinho) {
        QList<QStandardItem*> row;
        row.append(new QStandardItem(QString::number(item.produtoId)));
        row.append(new QStandardItem(item.nome));
        row.append(new QStandardItem(QString::number(item.quantidade)));
        row.append(new QStandardItem(QString::number(item.precoUnitario, 'f', 2)));
        row.append(new QStandardItem(QString::number(item.subtotal, 'f', 2)));
        m_cartModel->appendRow(row);
    }
}

double RegistroVendaWindow::calcularTotalCarrinho() const
{
    double total = 0.0;
    for (const auto& item : m_carrinho) {
        total += item.subtotal;
    }
    return total;
}

void RegistroVendaWindow::limparCarrinho()
{
    m_carrinho.clear();
    atualizarTabelaCarrinho();
    ui->textEdit_2->clear();
    on_textEdit_2_textChanged(); // Atualiza os estoques na tela de busca
}

void RegistroVendaWindow::on_pushButton_2_clicked() // Finalizar Venda
{
    if (m_carrinho.empty()) {
        QMessageBox::warning(this, "Aviso", "O carrinho está vazio.");
        return;
    }

    double total = calcularTotalCarrinho();
    
    // Como a UI não tem um local para o Total, confirmamos via MessageBox
    QString msg = QString("Valor Total: R$ %1\n\nDeseja finalizar a venda?")
                  .arg(total, 0, 'f', 2);
                  
    QMessageBox::StandardButton resposta = QMessageBox::question(this, "Confirmar Venda", msg, 
                                           QMessageBox::Yes | QMessageBox::No);

    if (resposta == QMessageBox::Yes) {
        QString errorMessage;
        if (m_salesService->finalizarVenda(m_carrinho, total, errorMessage)) {
            QMessageBox::information(this, "Sucesso", "Venda finalizada com sucesso!");
            limparCarrinho();
        } else {
            QMessageBox::critical(this, "Erro Crítico na Venda", errorMessage);
        }
    }
}
