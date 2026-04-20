#include "ProductService.h"
#include "../Core/SessionManager.h"
// #include "../DAO/ProdutoDAO.h"
// #include "../DAO/CategoriaDAO.h"

ProductService::ProductService(ProdutoDAO* produtoDao, CategoriaDAO* categoriaDao)
    : m_produtoDao(produtoDao), m_categoriaDao(categoriaDao) {}

bool ProductService::isGerenteLogado() const {
    // Conforme o PDF, APENAS o gerente cadastra/altera produtos
    return SessionManager::instance().isUserLoggedIn() && 
           SessionManager::instance().getTipo() == "gerente";
}

std::vector<Categoria> ProductService::listarCategorias() {
    std::vector<Categoria> categorias;
    // return m_categoriaDao->listAll();
    
    // Placeholder para testar a UI:
    categorias.push_back({1, "Eletrônicos"});
    categorias.push_back({2, "Ferramentas"});
    return categorias;
}

bool ProductService::cadastrarProduto(const QString& codigo, const QString& nome, const QString& descricao,
                                      double preco, int quantidade, int categoriaId, bool ativo, QString& errorMessage) {
    if (!isGerenteLogado()) {
        errorMessage = "Acesso negado: Apenas gerentes podem alterar produtos.";
        return false;
    }

    if (codigo.isEmpty() || nome.isEmpty() || categoriaId == -1) {
        errorMessage = "Código, nome e categoria são obrigatórios.";
        return false;
    }

    if (preco < 0 || quantidade < 0) {
        errorMessage = "Preço e quantidade não podem ser negativos.";
        return false;
    }

    // AQUI ENTRA O DAO (Pacote 3)
    // m_produtoDao->insert(...)
    return true; // Placeholder
}

bool ProductService::desativarProduto(int id, QString& errorMessage) {
    if (!isGerenteLogado()) {
        errorMessage = "Acesso negado.";
        return false;
    }
    // m_produtoDao->setActive(id, false);
    return true; // Placeholder
}

std::vector<Produto> ProductService::listarProdutos(const QString& buscaTexto, int categoriaId) {
    std::vector<Produto> lista;
    // m_produtoDao->search(buscaTexto, categoriaId);
    
    // Placeholder para visualização:
    lista.push_back({1, "001", "Parafusadeira", "12V Bateria", 250.50, 15, "Ferramentas", true});
    lista.push_back({2, "002", "Monitor 24", "Full HD", 800.00, 5, "Eletrônicos", false});
    return lista;
}
