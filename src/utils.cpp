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
#include <utility>
#include "../include/utils.hpp"

void preencher_grafos(TGrafo& g1, TGrafo& g2, TGrafo& g3) {
    // G1: Problema dos Exames (5 provas, 6 arestas, minimo de 3 dias).
    g1.insereA(0, 1);
    g1.insereA(0, 2);
    g1.insereA(1, 0);
    g1.insereA(1, 2);
    g1.insereA(1, 3);
    g1.insereA(2, 1);
    g1.insereA(2, 0);
    g1.insereA(2, 4);
    g1.insereA(3, 1);
    g1.insereA(3, 4);
    g1.insereA(4, 2);
    g1.insereA(4, 3);

    // G2: Hospital (9 enfermidades, 15 arestas, minimo de 4 quartos).
    g2.insereA(0, 5);
    g2.insereA(0, 6);
    g2.insereA(0, 8);
    g2.insereA(1, 2);
    g2.insereA(1, 7);
    g2.insereA(2, 1);
    g2.insereA(2, 4);
    g2.insereA(2, 7);
    g2.insereA(3, 4);
    g2.insereA(3, 5);
    g2.insereA(3, 7);
    g2.insereA(4, 2);
    g2.insereA(4, 3);
    g2.insereA(4, 6);
    g2.insereA(5, 0);
    g2.insereA(5, 3);
    g2.insereA(5, 6);
    g2.insereA(5, 8);
    g2.insereA(6, 0);
    g2.insereA(6, 4);
    g2.insereA(6, 5);
    g2.insereA(6, 7);
    g2.insereA(6, 8);
    g2.insereA(7, 1);
    g2.insereA(7, 2);
    g2.insereA(7, 3);
    g2.insereA(7, 6);
    g2.insereA(8, 0);
    g2.insereA(8, 5);
    g2.insereA(8, 6);

    // G3: 23 vertices, 71 arestas nao direcionadas e numero cromatico 5.
    std::vector<std::pair<int, int>> arestas = {
        {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}
    };
    int vertices = 5;

    for (int etapa = 0; etapa < 2; etapa++) {
        auto novas_arestas = arestas;
        // O clone de v recebe os vizinhos originais de v.
        for (const auto& aresta : arestas) {
            novas_arestas.emplace_back(aresta.first, vertices + aresta.second);
            novas_arestas.emplace_back(aresta.second, vertices + aresta.first);
        }
        // Um novo vertice se conecta a todos os clones.
        for (int v = 0; v < vertices; v++) {
            novas_arestas.emplace_back(vertices + v, 2 * vertices);
        }
        arestas = std::move(novas_arestas);
        vertices = 2 * vertices + 1;
    }

    for (const auto& aresta : arestas) {
        g3.insereA(aresta.first, aresta.second);
        g3.insereA(aresta.second, aresta.first);
    }
}

void print_vector_int(std::vector<int> v) {
    std::cout << "[";

    for (std::size_t i = 0; i < v.size(); i++) {
        if (i > 0)
            std::cout << ", ";
        std::cout << v[i];
    }

    std::cout << "]" << std::endl;
}

void imprimir_coloracao(const char* nome, const std::vector<std::vector<int>>& cores) {
    std::cout << "\n" << nome << std::endl;
    int quantidade = 0;

    for (const auto& classe : cores) {
        if (classe.empty())
            continue;

        std::cout << "Cor " << ++quantidade << ": ";
        print_vector_int(classe);
    }

    std::cout << "Total de cores utilizadas: " << quantidade << std::endl;
}
