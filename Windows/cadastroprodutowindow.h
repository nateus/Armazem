#ifndef CADASTROPRODUTOWINDOW_H
#define CADASTROPRODUTOWINDOW_H

#include <QWidget>
class ProductService; // Forward declaration

namespace Ui {
class CadastroProdutoWindow;
}

class CadastroProdutoWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CadastroProdutoWindow(ProductService* productService, QWidget *parent = nullptr);
    ~CadastroProdutoWindow();

signals:
    void windowClosed();

private slots:
    void on_pushButton_clicked(); // Botão Salvar

private:
    Ui::CadastroProdutoWindow *ui;
    ProductService* m_productService;

    void carregarCategorias();
};

#endif // CADASTROPRODUTOWINDOW_H
