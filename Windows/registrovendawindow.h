#ifndef REGISTROVENDAWINDOW_H
#define REGISTROVENDAWINDOW_H

#include <QWidget>
#include <QStandardItemModel>
#include <vector>
#include "../Services/SalesService.h" // Precisa importar as structs

namespace Ui {
class RegistroVendaWindow;
}

class RegistroVendaWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RegistroVendaWindow(SalesService* salesService, QWidget *parent = nullptr);
    ~RegistroVendaWindow();

private slots:
    void on_textEdit_2_textChanged();   // Buscar produtos
    void on_pushButton_clicked();       // Adicionar ao carrinho
    void on_pushButton_3_clicked();     // Remover item do carrinho
    void on_pushButton_2_clicked();     // Finalizar Venda

private:
    Ui::RegistroVendaWindow *ui;
    SalesService* m_salesService;
    
    QStandardItemModel* m_searchModel;
    QStandardItemModel* m_cartModel;
    std::vector<ItemCarrinho> m_carrinho;

    void atualizarTabelaCarrinho();
    double calcularTotalCarrinho() const;
    void limparCarrinho();
};

#endif // REGISTROVENDAWINDOW_H
