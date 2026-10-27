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

#include <iostream>
#include <stack>
#include <queue>
#include "../include/TGrafo.hpp"

bool _vector_find(const std::vector<int>& v, const int num) {
    for (auto i : v) {
        if (i == num)
            return true;
    }

    return false;
}

/*
    Construtor do TGrafo, responsável por 
    Criar a matriz de adjacência v x v do Grafo
*/
TGrafo::TGrafo( int n ){
    this->n = n;

    // No início dos tempos não há arestas
    this->m = 0; 

    // aloca da matriz do TGrafo
    int **adjac = new int*[n];

    for(int i = 0; i < n; i++)
        adjac[i]= new int[n];

    adj = adjac;

    // Inicia a matriz com zeros
    for(int i = 0; i< n; i++)
        for(int j = 0; j< n; j++)
            adj[i][j]=0;    
}

/*
    Destructor, responsável por
    liberar a memória alocada para a matriz
*/
TGrafo::~TGrafo(){
    n = 0;
    m = 0;

    delete [] *adj;

    std::cout << "espaco liberado" << std::endl;
}

/*
    Insere uma aresta no Grafo tal que
    v é adjacente a w
*/
void TGrafo::insereA( int v, int w){
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
    // testa se temos a aresta
    if(adj[v][w] == 1){
        adj[v][w] = 0;
        m--; // atualiza qtd arestas
    }
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
    std::cout << "   ";

    for(int w = 0; w < n; w++){
        if(w > 0)
            std::cout << " ";
        std::cout << w;
    }

    std::cout << std::endl;

    for(int i = 0; i < n; i++){
        std::cout << i << " [";

        for(int w = 0; w < n; w++){
            if(w > 0)
                std::cout << " ";
            std::cout << adj[i][w];
        }

        std::cout << "]" << std::endl;
    }
}

std::vector<int> TGrafo::busca_profundidade(int inicial) {
    std::vector<int> visitados;
    std::stack<int> stack;
    int no = inicial;

    visitados.push_back(no);
    stack.push(no);

    while (!stack.empty()) {
        no = stack.top();
        stack.pop();

        for (int i = 0; i < this->n; i++) {
            if (this->adj[no][i] == 0 || _vector_find(visitados, i))
                continue;

            visitados.push_back(i);
            stack.push(no);

            no = i;
        }
    }

    return visitados;
}

std::vector<int> TGrafo::busca_largura(int inicial) {
    std::vector<int> visitados;
    std::queue<int> queue;
    int no = inicial;

    visitados.push_back(no);
    queue.push(no);

    while (!queue.empty()) {
        no = queue.front();
        queue.pop();

        for (int i = 0; i < this->n; i++) {
            if (this->adj[no][i] == 0 || _vector_find(visitados, i))
                continue;

            visitados.push_back(i);
            queue.push(i);
        }
    }

    return visitados;
}