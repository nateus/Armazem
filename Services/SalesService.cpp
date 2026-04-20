#include "SalesService.h"
#include "../Core/SessionManager.h"
// Headers do Pacote 3
// #include "../DAO/ProdutoDAO.h"
// #include "../DAO/VendaDAO.h"
// #include "../DAO/MovimentacaoDAO.h"
// #include "../Database/DatabaseManager.h"

SalesService::SalesService(ProdutoDAO* produtoDao, VendaDAO* vendaDao, 
                           MovimentacaoDAO* movimentacaoDao, DatabaseManager* dbManager)
    : m_produtoDao(produtoDao), m_vendaDao(vendaDao), 
      m_movimentacaoDao(movimentacaoDao), m_dbManager(dbManager) {}

bool SalesService::isFuncionarioValido() const {
    return SessionManager::instance().isUserLoggedIn();
}

std::vector<ProdutoBusca> SalesService::buscarProdutosAtivos(const QString& termoBusca) {
    std::vector<ProdutoBusca> lista;
    // m_produtoDao->searchActiveWithStock(termoBusca);
    
    // Placeholder para testar a integração visual
    lista.push_back({1, "Arroz 5kg", 25.50, 100});
    lista.push_back({2, "Feijão 1kg", 8.90, 50});
    return lista;
}

bool SalesService::finalizarVenda(const std::vector<ItemCarrinho>& carrinho, double total, QString& errorMessage) {
    if (!isFuncionarioValido()) {
        errorMessage = "Sessão inválida. Faça login novamente.";
        return false;
    }

    if (carrinho.empty()) {
        errorMessage = "A venda deve conter pelo menos um item.";
        return false;
    }

    int usuarioLogadoId = SessionManager::instance().getUserId();

    // INÍCIO DA TRANSAÇÃO (Pacote 3)
    // m_dbManager->beginTransaction();

    try {
        // 1. Gravar na tabela 'vendas' e recuperar o ID gerado
        // int vendaId = m_vendaDao->insertVenda(usuarioLogadoId, total);
        int vendaId = 999; // Mock

        for (const auto& item : carrinho) {
            // 2. Gravar na tabela 'itens_venda'
            // m_vendaDao->insertItem(vendaId, item.produtoId, item.quantidade, item.precoUnitario, item.subtotal);

            // 3. Validar estoque real no momento exato da venda para evitar "Race Condition"
            // int estoqueAtual = m_produtoDao->getQuantidade(item.produtoId);
            int estoqueAtual = 100; // Mock
            
            if (estoqueAtual < item.quantidade) {
                throw std::runtime_error("Estoque insuficiente para o produto: " + item.nome.toStdString());
            }

            // 4. Baixa Automática no Estoque
            // m_produtoDao->updateQuantidade(item.produtoId, estoqueAtual - item.quantidade);

            // 5. Registrar Histórico de Movimentação (Saída)
            // m_movimentacaoDao->insert(item.produtoId, usuarioLogadoId, "saida", item.quantidade, "Venda #" + QString::number(vendaId));
        }

        // SE TUDO DEU CERTO, CONFIRMA NO BANCO:
        // m_dbManager->commitTransaction();
        return true;

    } catch (const std::exception& e) {
        // SE QUALQUER PASSO FALHAR, DESFAZ TUDO (Rollback Total)
        // m_dbManager->rollbackTransaction();
        errorMessage = QString::fromStdString(e.what());
        return false;
    }
}
