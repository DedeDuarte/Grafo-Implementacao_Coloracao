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

#ifndef UTILS_HPP
#define UTILS_HPP

#include "../include/TGrafo.hpp"

void preencher_grafos(TGrafo& g1, TGrafo& g2, TGrafo& g3);
void print_vector_int(std::vector<int> v);
void imprimir_coloracao(const char* nome, const std::vector<std::vector<int>>& cores);

#endif
