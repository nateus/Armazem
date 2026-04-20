#ifndef MOVIMENTACAOESTOQUEWINDOW_H
#define MOVIMENTACAOESTOQUEWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

class InventoryService; // Forward declaration

namespace Ui {
class MovimentacaoEstoqueWindow;
}

class MovimentacaoEstoqueWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MovimentacaoEstoqueWindow(InventoryService* inventoryService, QWidget *parent = nullptr);
    ~MovimentacaoEstoqueWindow();

private slots:
    void carregarProdutos();
    void on_textEdit_textChanged(); // Campo de busca
    void on_pushButton_clicked();   // Botão Confirmar

private:
    Ui::MovimentacaoEstoqueWindow *ui;
    InventoryService* m_inventoryService;
    QStandardItemModel* m_tableModel;

    int getSelectedProductId();
    void limparFormulario();
};

#endif // MOVIMENTACAOESTOQUEWINDOW_H
