#ifndef PRODUCTSERVICE_H
#define PRODUCTSERVICE_H

#include <QString>
#include <vector>

// Forward declarations dos DAOs (Pacote 3)
class ProdutoDAO;
class CategoriaDAO;

// Placeholders dos Modelos (Pacote 2) para compilar
struct Categoria {
    int id;
    QString nome;
};

struct Produto {
    int id;
    QString codigo;
    QString nome;
    QString descricao;
    double preco;
    int quantidade;
    QString categoriaNome; // Simplificado para a view
    bool ativo;
};

class ProductService {
public:
    ProductService(ProdutoDAO* produtoDao, CategoriaDAO* categoriaDao);

    bool cadastrarProduto(const QString& codigo, const QString& nome, const QString& descricao,
                          double preco, int quantidade, int categoriaId, bool ativo, QString& errorMessage);
    
    bool desativarProduto(int id, QString& errorMessage);
    
    std::vector<Produto> listarProdutos(const QString& buscaTexto = "", int categoriaId = -1);
    std::vector<Categoria> listarCategorias();

private:
    ProdutoDAO* m_produtoDao;
    CategoriaDAO* m_categoriaDao;

    bool isGerenteLogado() const; // Validação de perfil
};

#endif // PRODUCTSERVICE_H
