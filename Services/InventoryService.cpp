#include "InventoryService.h"
#include "../Core/SessionManager.h"
// #include "../DAO/ProdutoDAO.h"
// #include "../DAO/MovimentacaoDAO.h"

InventoryService::InventoryService(ProdutoDAO* produtoDao, MovimentacaoDAO* movimentacaoDao)
    : m_produtoDao(produtoDao), m_movimentacaoDao(movimentacaoDao) {}

bool InventoryService::isGerenteLogado() const {
    // Regra Obrigatória: Apenas gerente acessa
    return SessionManager::instance().isUserLoggedIn() && 
           SessionManager::instance().getTipo() == "gerente";
}

std::vector<ProdutoEstoque> InventoryService::listarProdutosParaEstoque(const QString& buscaTexto) {
    std::vector<ProdutoEstoque> lista;
    if (!isGerenteLogado()) return lista;

    // m_produtoDao->search(buscaTexto, -1);
    
    // Placeholder para testar a UI:
    lista.push_back({1, "001", "Parafusadeira 12V", 15});
    lista.push_back({2, "002", "Monitor 24 Polegadas", 5});
    return lista;
}

bool InventoryService::registrarMovimentacao(int produtoId, const QString& tipo, int quantidadeInformada, const QString& motivo, QString& errorMessage) {
    if (!isGerenteLogado()) {
        errorMessage = "Acesso negado: Apenas gerentes podem movimentar o estoque.";
        return false;
    }

    if (quantidadeInformada <= 0 && tipo != "ajuste") {
        errorMessage = "A quantidade deve ser maior que zero.";
        return false;
    }

    if (motivo.isEmpty()) {
        errorMessage = "O motivo da movimentação é obrigatório.";
        return false;
    }

    // 1. Obter o estoque atual do produto no banco (via DAO)
    // int estoqueAtual = m_produtoDao->getQuantidade(produtoId);
    int estoqueAtual = 15; // Valor mockado para teste

    int novoEstoque = estoqueAtual;
    int quantidadeParaHistorico = quantidadeInformada;

    // 2. Aplicar as regras de cálculo
    if (tipo == "entrada") {
        novoEstoque = estoqueAtual + quantidadeInformada;
    } 
    else if (tipo == "saida") {
        novoEstoque = estoqueAtual - quantidadeInformada;
        if (novoEstoque < 0) {
            errorMessage = "Estoque insuficiente. A saída deixaria o estoque negativo.";
            return false; // Regra Obrigatória
        }
    } 
    else if (tipo == "ajuste") {
        // No ajuste, a quantidade informada é o NOVO valor exato do estoque.
        novoEstoque = quantidadeInformada;
        // Se o estoque era 10 e ajustou para 8, a diferença é 2.
        quantidadeParaHistorico = std::abs(novoEstoque - estoqueAtual);
        
        if (novoEstoque == estoqueAtual) {
            errorMessage = "O valor ajustado é igual ao estoque atual.";
            return false;
        }
    } else {
        errorMessage = "Tipo de movimentação inválido.";
        return false;
    }

    // 3. Persistência (Pacote 3 cuidará da Transação no DatabaseManager)
    int usuarioLogadoId = SessionManager::instance().getUserId();

    // m_produtoDao->updateQuantidade(produtoId, novoEstoque);
    // m_movimentacaoDao->insert(produtoId, usuarioLogadoId, tipo, quantidadeParaHistorico, motivo);

    return true; // Placeholder
}
