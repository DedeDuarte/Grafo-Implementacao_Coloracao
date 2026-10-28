/**
 * Implementação:
 * Coloração sequencial de vértices (Grupo do Projeto)
 * 
 * Integrantes:
 * |=================================|==========|
 * |               Nome              |    RA    |
 * |---------------------------------|----------|
 * | André Doerner Duarte            | 10427938 |
 * | Matheus Leonardo Cardoso Kroeff | 10426434 |
 * | Naoto Ushizaki                  | 10437445 |
 * |=================================|==========|
 */

/*
Implementação de uma Classe para grafos denominada TGrafo,
usando Matriz de Adjacência
e métodos para utilização de um grafo dirigido.
*/
#ifndef ___GRAFO_MATRIZ_ADJACENCIA___
#define ___GRAFO_MATRIZ_ADJACENCIA___

#include <vector>

// definição de uma estrutura para armezanar um grafo
// Também seria possível criar um arquivo grafo.h 
// e fazer a inclusão "#include <grafo.h>"
class TGrafo{
private:
    int n; // quantidade de vértices
    int m; // quantidade de arestas
    int **adj; //matriz de adjacência
    void validarVertices(int v, int w) const;

public:
    TGrafo(int n);
    // A matriz pertence exclusivamente a este objeto (sem cópia superficial).
    TGrafo(const TGrafo&) = delete;
    TGrafo& operator=(const TGrafo&) = delete;
    void insereA(int v, int w);
    void removeA(int v, int w);
    bool existeA(int v, int w) const;
    void show();
    void show_();
    std::vector<std::vector<int>> coloracao();
    ~TGrafo();
};

#endif