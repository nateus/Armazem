#ifndef GERENCIARPRODUTOSWINDOW_H
#define GERENCIARPRODUTOSWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

class ProductService;

namespace Ui {
class GerenciarProdutosWindow;
}

class GerenciarProdutosWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GerenciarProdutosWindow(ProductService* productService, QWidget *parent = nullptr);
    ~GerenciarProdutosWindow();

private slots:
    void carregarDados();
    void carregarCategoriasFiltro();
    
    void on_pushButton_4_clicked(); // Atualizar
    void on_pushButton_2_clicked(); // Remover/Desativar
    void on_textEdit_textChanged(); // Buscar
    void on_comboBox_currentIndexChanged(int index); // Filtrar por Categoria

private:
    Ui::GerenciarProdutosWindow *ui;
    ProductService* m_productService;
    QStandardItemModel* m_tableModel;

    int getSelectedProductId();
};

#endif // GERENCIARPRODUTOSWINDOW_H
