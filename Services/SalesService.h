#ifndef SALESSERVICE_H
#define SALESSERVICE_H

#include <QString>
#include <vector>

// Forward declarations do Pacote 3
class ProdutoDAO;
class VendaDAO;
class MovimentacaoDAO;
class DatabaseManager;

// Estruturas de dados para o fluxo da janela
struct ProdutoBusca {
    int id;
    QString nome;
    double preco;
    int estoqueDisponivel;
};

struct ItemCarrinho {
    int produtoId;
    QString codigo;
    QString nome;
    double precoUnitario;
    int quantidade;
    double subtotal;
};

class SalesService {
public:
    SalesService(ProdutoDAO* produtoDao, VendaDAO* vendaDao, 
                 MovimentacaoDAO* movimentacaoDao, DatabaseManager* dbManager);

    // Retorna apenas produtos ATIVOS e com estoque > 0
    std::vector<ProdutoBusca> buscarProdutosAtivos(const QString& termoBusca);
    
    // Valida e processa a venda no banco usando Transação
    bool finalizarVenda(const std::vector<ItemCarrinho>& carrinho, double total, QString& errorMessage);

private:
    ProdutoDAO* m_produtoDao;
    VendaDAO* m_vendaDao;
    MovimentacaoDAO* m_movimentacaoDao;
    DatabaseManager* m_dbManager;

    bool isFuncionarioValido() const;
};

#endif // SALESSERVICE_H
