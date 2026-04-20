#include "cadastroprodutowindow.h"
#include "ui_cadastroprodutowindow.h"
#include "../Services/ProductService.h"
#include <QMessageBox>

CadastroProdutoWindow::CadastroProdutoWindow(ProductService* productService, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CadastroProdutoWindow)
    , m_productService(productService)
{
    ui->setupUi(this);
    ui->checkBox->setChecked(true); // Padrão: Ativo
    
    carregarCategorias();
}

CadastroProdutoWindow::~CadastroProdutoWindow()
{
    delete ui;
}

void CadastroProdutoWindow::carregarCategorias()
{
    ui->comboBox->clear();
    ui->comboBox->addItem("Selecione...", -1); // Opção nula

    std::vector<Categoria> categorias = m_productService->listarCategorias();
    for (const auto& cat : categorias) {
        // Guarda o nome para exibição e o ID como dado oculto
        ui->comboBox->addItem(cat.nome, cat.id);
    }
}

void CadastroProdutoWindow::on_pushButton_clicked()
{
    QString codigo = ui->textEdit->toPlainText().trimmed();
    QString nome = ui->textEdit_2->toPlainText().trimmed();
    QString descricao = ui->textEdit_3->toPlainText().trimmed();
    double preco = ui->doubleSpinBox->value();
    int quantidade = ui->spinBox->value();
    
    // Recupera o ID oculto da categoria selecionada
    int categoriaId = ui->comboBox->currentData().toInt();
    bool ativo = ui->checkBox->isChecked();

    QString errorMessage;
    if (m_productService->cadastrarProduto(codigo, nome, descricao, preco, quantidade, categoriaId, ativo, errorMessage)) {
        QMessageBox::information(this, "Sucesso", "Produto cadastrado com sucesso!");
        emit windowClosed();
        this->close();
    } else {
        QMessageBox::warning(this, "Erro", errorMessage);
    }
}
