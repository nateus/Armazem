#ifndef INVENTORYSERVICE_H
#define INVENTORYSERVICE_H

#include <QString>
#include <vector>

// Forward declarations dos DAOs (Pacote 3)
class ProdutoDAO;
class MovimentacaoDAO;

// Placeholder para renderizar a tabela na UI
struct ProdutoEstoque {
    int id;
    QString codigo;
    QString nome;
    int quantidadeAtual;
};

class InventoryService {
public:
    InventoryService(ProdutoDAO* produtoDao, MovimentacaoDAO* movimentacaoDao);

    std::vector<ProdutoEstoque> listarProdutosParaEstoque(const QString& buscaTexto = "");
    
    // O núcleo da regra de negócio deste pacote
    bool registrarMovimentacao(int produtoId, const QString& tipo, int quantidadeInformada, const QString& motivo, QString& errorMessage);

private:
    ProdutoDAO* m_produtoDao;
    MovimentacaoDAO* m_movimentacaoDao;

    bool isGerenteLogado() const; // Validação de perfil
};

#endif // INVENTORYSERVICE_H
