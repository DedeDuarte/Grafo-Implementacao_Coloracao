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

#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
#include <vector>
#include "../include/TGrafo.hpp"

/*
    Construtor do TGrafo, responsável por 
    Criar a matriz de adjacência v x v do Grafo
*/
TGrafo::TGrafo( int n ){
    if (n < 0)
        throw std::invalid_argument("A quantidade de vertices nao pode ser negativa");

    this->n = n;

    // No início dos tempos não há arestas
    this->m = 0; 

    // aloca da matriz do TGrafo
    int **adjac = new int*[n]();

    try {
        // Inicia cada linha com zeros.
        for (int i = 0; i < n; i++)
            adjac[i] = new int[n]();
    } catch (...) {
        // O destrutor não é chamado quando a construção falha.
        for (int i = 0; i < n; i++)
            delete[] adjac[i];
        delete[] adjac;
        throw;
    }

    adj = adjac;
}

/*
    Destructor, responsável por
    liberar a memória alocada para a matriz
*/
TGrafo::~TGrafo(){
    for (int i = 0; i < n; i++)
        delete[] adj[i];

    delete[] adj;

    std::cout << "espaco liberado" << std::endl;
}

/*
    Insere uma aresta no Grafo tal que
    v é adjacente a w
*/
void TGrafo::insereA( int v, int w){
    validarVertices(v, w);
    // testa se nao temos a aresta
    if(adj[v][w] == 0){
        adj[v][w] = 1;
        m++; // atualiza qtd arestas
    }
}

/*
    Remove uma aresta v->w do Grafo
*/
void TGrafo::removeA(int v, int w){
    validarVertices(v, w);
    // testa se temos a aresta
    if(adj[v][w] == 1){
        adj[v][w] = 0;
        m--; // atualiza qtd arestas
    }
}

void TGrafo::validarVertices(int v, int w) const {
    if (v < 0 || w < 0 || v >= n || w >= n)
        throw std::out_of_range("Indice de vertice fora do grafo");
}

bool TGrafo::existeA(int v, int w) const {
    validarVertices(v, w);
    return adj[v][w] != 0;
}

/*
    Apresenta o Grafo contendo
    número de vértices, arestas
    e a matriz de adjacência obtida
*/
void TGrafo::show_(){
    std::cout << "n: " << n << std::endl;
    std::cout << "m: " << m << std::endl;

    for (int i=0; i < n; i++){
        std::cout << "\n";

        for (int w=0; w < n; w++)
            if (adj[i][w] == 1)
                std::cout << "Adj[" << i<< "," << w << "]= 1" << " ";
            else
                std::cout << "Adj[" << i<< "," << w << "]= 0" << " ";
    }
    std::cout << "\nfim da impressao do grafo." << std::endl;
}

/*
    Apresenta a matriz de adjacencia em formato tabular
*/
void TGrafo::show(){
    const int largura_rotulo = static_cast<int>(std::to_string(n > 0 ? n - 1 : 0).size());
    const int largura_coluna = largura_rotulo + 1;

    std::cout << std::string(largura_rotulo + 2, ' ');
    for (int w = 0; w < n; w++)
        std::cout << std::setw(w == 0 ? 1 : largura_coluna) << w;
    std::cout << std::endl;

    for (int i = 0; i < n; i++) {
        std::cout << std::setw(largura_rotulo) << i << " [";
        for (int w = 0; w < n; w++)
            std::cout << std::setw(w == 0 ? 1 : largura_coluna) << adj[i][w];
        std::cout << "]" << std::endl;
    }
} 

/*
    Coloração sequencial
*/
std::vector<std::vector<int>> TGrafo::coloracao() {
    // Um laço torna impossível uma coloração própria.
    for (int i = 0; i < n; i++)
        if (adj[i][i])
            throw std::domain_error("Nao existe coloracao propria para grafo com laco");

    std::vector<std::vector<int>> cores(this->n);

    int cor = 0;

    // Para cada vertice
    for (int i = 0; i < this->n; i++) {
        bool conflito = true;

        // Enquanto nao encontrar uma cor diferente dos seus vizinhos
        while (conflito) {
            conflito = false;

            // Procura algum vizinho em cores[cor]
            for (int j : cores[cor]) {
                // Uma aresta em qualquer sentido impede compartilhar a cor.
                if (this->adj[i][j] || this->adj[j][i]) { // Se achar, pula para a proxima cor
                    conflito = true;
                    cor++;
                    break;
                }
            }
        }

        cores[cor].push_back(i);

        cor = 0;
    }

    return cores;
}