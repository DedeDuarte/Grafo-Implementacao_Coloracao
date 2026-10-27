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
#include "TGrafo.hpp"
#include "utils.hpp"

int main() {
    // Crinado grafos
    TGrafo g1(5);
    TGrafo g2(9);
    TGrafo g3(23);

    preencher_grafos(g1, g2, g3);

    // Conferindo grafos
    std::cout << "===== Grafos Gerados =====" << std::endl;
    std::cout << "Grafo G1 - Exames (0=M, 1=P, 2=I, 3=B, 4=H):" << std::endl;
    g1.show();

    std::cout << "\nGrafo G2 - Hospital (0=A, 1=B, 2=C, 3=D, 4=E, 5=F, 6=G, 7=H, 8=I):" << std::endl;
    g2.show();

    std::cout << "\nGrafo G3 (numero cromatico esperado: 5):" << std::endl;
    g3.show();

    // Testes de coloracao sequencial
    auto cores_g1 = g1.coloracao();
    auto cores_g2 = g2.coloracao();
    auto cores_g3 = g3.coloracao();

    std::cout << "\n===== Testes de Coloracao Sequencial =====" << std::endl;
    imprimir_coloracao("G1 - Exames (minimo: 3 dias)", cores_g1);
    imprimir_coloracao("G2 - Hospital (minimo: 4 quartos)", cores_g2);
    imprimir_coloracao("G3 (minimo: 5 cores)", cores_g3);

    std::cout << "\nFinalizando código..." << std::endl;

    return 0;
}