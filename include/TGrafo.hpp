/**
 * Implementação:
 * Busca em Largura e Profundidade (Grupo do Projeto)
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
    public:
        TGrafo(int n);
        void insereA(int v, int w);
        void removeA(int v, int w);
        void show();
        void show_();
        std::vector<int> busca_profundidade(int inicial);
        std::vector<int> busca_largura(int inicial);
        ~TGrafo();
};

#endif