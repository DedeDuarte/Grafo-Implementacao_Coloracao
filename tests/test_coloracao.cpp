#include <algorithm>
#include <climits>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include "TGrafo.hpp"
#include "utils.hpp"

static_assert(!std::is_copy_constructible<TGrafo>::value,
              "TGrafo nao pode copiar a matriz superficialmente");
static_assert(!std::is_copy_assignable<TGrafo>::value,
              "TGrafo nao pode compartilhar a propriedade da matriz");
static_assert(!std::is_move_constructible<TGrafo>::value,
              "TGrafo nao possui transferencia de propriedade implementada");
static_assert(!std::is_move_assignable<TGrafo>::value,
              "TGrafo nao possui atribuicao por movimento implementada");

namespace {
using Cores = std::vector<std::vector<int>>;

void exigir(bool condicao, const std::string& mensagem) {
    if (!condicao)
        throw std::runtime_error(mensagem);
}

template <typename Excecao, typename Funcao>
void exigir_excecao(Funcao acao, const std::string& mensagem) {
    try {
        acao();
    } catch (const Excecao&) {
        return;
    } catch (const std::exception& erro) {
        throw std::runtime_error(mensagem + ": excecao inesperada: " + erro.what());
    }
    throw std::runtime_error(mensagem + ": nenhuma excecao foi lancada");
}

// Valida a partição e cada aresta sem depender da quantidade esperada de cores.
void validar_coloracao(const TGrafo& grafo, int n, const Cores& cores) {
    std::vector<int> cor_por_vertice(n, -1);
    for (std::size_t cor = 0; cor < cores.size(); ++cor) {
        for (int v : cores[cor]) {
            exigir(v >= 0 && v < n, "Indice invalido na coloracao: " + std::to_string(v));
            exigir(cor_por_vertice[v] == -1,
                   "Vertice repetido na coloracao: " + std::to_string(v));
            cor_por_vertice[v] = static_cast<int>(cor);
        }
    }
    for (int v = 0; v < n; ++v) {
        exigir(cor_por_vertice[v] != -1, "Vertice sem cor: " + std::to_string(v));
        for (int w = 0; w < n; ++w) {
            if (grafo.existeA(v, w))
                exigir(cor_por_vertice[v] != cor_por_vertice[w],
                       "Vertices adjacentes com a mesma cor: " +
                       std::to_string(v) + " e " + std::to_string(w));
        }
    }
}

int contar_cores(const Cores& cores) {
    return static_cast<int>(std::count_if(cores.begin(), cores.end(),
                                         [](const auto& cor) { return !cor.empty(); }));
}

void verificar(TGrafo& grafo, int n, int quantidade) {
    const auto cores = grafo.coloracao();
    validar_coloracao(grafo, n, cores);
    exigir(contar_cores(cores) == quantidade,
           "Esperadas " + std::to_string(quantidade) + " cores, obtidas " +
           std::to_string(contar_cores(cores)));
    exigir(grafo.coloracao() == cores, "Coloracao mudou entre chamadas sem alterar o grafo");
}

void ligar(TGrafo& grafo, int v, int w) {
    grafo.insereA(v, w);
    grafo.insereA(w, v);
}

void testar_vazio() {
    TGrafo grafo(0);
    verificar(grafo, 0, 0);
    exigir(grafo.coloracao().empty(), "Grafo vazio deve retornar um vetor vazio");
    exigir_excecao<std::out_of_range>([&] { grafo.insereA(0, 0); }, "Insercao em grafo vazio");
    exigir_excecao<std::out_of_range>([&] { grafo.removeA(0, 0); }, "Remocao em grafo vazio");
    exigir_excecao<std::out_of_range>([&] { grafo.existeA(0, 0); }, "Consulta em grafo vazio");
}

void testar_isolado() {
    TGrafo grafo(1);
    verificar(grafo, 1, 1);
}

void testar_sem_arestas() {
    TGrafo grafo(8);
    verificar(grafo, 8, 1);
}

void testar_completo() {
    TGrafo grafo(6);
    for (int v = 0; v < 6; ++v)
        for (int w = v + 1; w < 6; ++w)
            ligar(grafo, v, w);
    verificar(grafo, 6, 6);
}

void testar_bipartido() {
    TGrafo grafo(7);
    for (int v = 0; v < 3; ++v)
        for (int w = 3; w < 7; ++w)
            ligar(grafo, v, w);
    verificar(grafo, 7, 2);
}

void testar_ciclo_impar() {
    TGrafo grafo(5);
    for (int v = 0; v < 5; ++v)
        ligar(grafo, v, (v + 1) % 5);
    verificar(grafo, 5, 3);
}

void testar_guloso_nao_minimo() {
    // Grafo coroa bipartido: a ordem intercalada força o guloso a usar 3 cores.
    TGrafo grafo(6);
    for (int v = 0; v < 3; ++v)
        for (int w = 0; w < 3; ++w)
            if (v != w)
                ligar(grafo, 2 * v, 2 * w + 1);
    verificar(grafo, 6, 3);
}

void testar_duplicatas_e_remocao() {
    TGrafo grafo(2);
    ligar(grafo, 0, 1);
    const auto original = grafo.coloracao();
    ligar(grafo, 0, 1);
    exigir(grafo.coloracao() == original, "Aresta duplicada alterou a coloracao");
    verificar(grafo, 2, 2);
    grafo.removeA(0, 1);
    grafo.removeA(0, 1);
    exigir(!grafo.existeA(0, 1) && grafo.existeA(1, 0), "Remocao deve afetar apenas v->w");
    verificar(grafo, 2, 2);
    grafo.removeA(1, 0);
    exigir(!grafo.existeA(1, 0), "Aresta nao foi removida");
    verificar(grafo, 2, 1);
    // A contagem de arcos também deve permanecer correta após duplicatas.
    std::ostringstream saida;
    auto* anterior = std::cout.rdbuf(saida.rdbuf());
    grafo.show_();
    std::cout.rdbuf(anterior);
    exigir(saida.str().find("n: 2\nm: 0\n") == 0, "Contagem de arcos incorreta apos remocoes");
}

void testar_indices_invalidos() {
    TGrafo grafo(3);
    ligar(grafo, 0, 1);
    const auto antes = grafo.coloracao();
    const std::vector<std::pair<int, int>> indices = {
        {-1, 0}, {0, -1}, {3, 0}, {0, 3}, {3, 3},
        {INT_MIN, 1}, {1, INT_MIN}, {INT_MAX, 1}, {1, INT_MAX}
    };
    for (const auto& par : indices) {
        const int v = par.first;
        const int w = par.second;
        const std::string contexto = "(" + std::to_string(v) + ", " + std::to_string(w) + ")";
        exigir_excecao<std::out_of_range>([&] { grafo.insereA(v, w); }, "Insercao " + contexto);
        exigir_excecao<std::out_of_range>([&] { grafo.removeA(v, w); }, "Remocao " + contexto);
        exigir_excecao<std::out_of_range>([&] { grafo.existeA(v, w); }, "Consulta " + contexto);
    }
    exigir(grafo.coloracao() == antes, "Operacao invalida modificou o grafo");
    exigir(grafo.existeA(0, 1) && grafo.existeA(1, 0), "Operacao invalida removeu uma aresta");
    verificar(grafo, 3, 2);
}

void testar_construtor_invalido() {
    for (int n : {-1, -10, INT_MIN})
        exigir_excecao<std::invalid_argument>([&] { TGrafo grafo(n); }, "Quantidade negativa de vertices");
}

void testar_lacos() {
    TGrafo grafo(3);
    ligar(grafo, 0, 1);
    for (int v = 0; v < 3; ++v) {
        grafo.insereA(v, v);
        grafo.insereA(v, v);
        exigir(grafo.existeA(v, v), "Laco deve ficar registrado na matriz");
        exigir_excecao<std::domain_error>([&] { grafo.coloracao(); }, "Coloracao com laco");
        grafo.removeA(v, v);
        exigir(!grafo.existeA(v, v), "Laco nao foi removido");
        verificar(grafo, 3, 2);
    }
}

void testar_arestas_dirigidas() {
    for (const auto& aresta : {std::pair<int, int>{0, 1}, {1, 0}}) {
        TGrafo grafo(2);
        grafo.insereA(aresta.first, aresta.second);
        const TGrafo& consulta = grafo;
        exigir(consulta.existeA(aresta.first, aresta.second), "Aresta dirigida nao inserida");
        exigir(!consulta.existeA(aresta.second, aresta.first), "Insercao criou uma aresta reversa");
        verificar(grafo, 2, 2);
    }
}

void verificar_exemplo(TGrafo& grafo, int n, int arestas, int cores) {
    int arcos = 0;
    for (int v = 0; v < n; ++v) {
        exigir(!grafo.existeA(v, v), "Exemplo academico contem laco");
        for (int w = 0; w < n; ++w) {
            exigir(grafo.existeA(v, w) == grafo.existeA(w, v), "Exemplo perdeu simetria");
            arcos += grafo.existeA(v, w) ? 1 : 0;
        }
    }
    exigir(arcos == 2 * arestas, "Quantidade incorreta de arestas no exemplo academico");
    verificar(grafo, n, cores);
}

void testar_exemplos() {
    TGrafo g1(5), g2(9), g3(23);
    preencher_grafos(g1, g2, g3);
    try {
        verificar_exemplo(g1, 5, 6, 3);
    } catch (const std::exception& erro) {
        throw std::runtime_error(std::string("G1: ") + erro.what());
    }
    try {
        verificar_exemplo(g2, 9, 15, 4);
    } catch (const std::exception& erro) {
        throw std::runtime_error(std::string("G2: ") + erro.what());
    }
    try {
        verificar_exemplo(g3, 23, 71, 5);
    } catch (const std::exception& erro) {
        throw std::runtime_error(std::string("G3: ") + erro.what());
    }
}

void testar_impressao_vetor_vazio() {
    std::ostringstream saida;
    auto* anterior = std::cout.rdbuf(saida.rdbuf());
    print_vector_int({});
    std::cout.rdbuf(anterior);
    exigir(saida.str() == "[]\n", "Vetor vazio deve ser impresso como []");
}

void testar_validador() {
    TGrafo grafo(3);
    ligar(grafo, 0, 1);
    // Todas possuem duas classes não vazias: contar cores não detectaria os erros.
    const std::vector<Cores> invalidas = {
        {{0}, {1}},             // Falta o vértice 2.
        {{0, 2}, {1, 2}},       // Mesmo vértice em classes distintas.
        {{0, 0, 2}, {1}},       // Repetição na mesma classe.
        {{0, 1}, {2}},          // Adjacentes na mesma classe.
        {{0, 2, -1}, {1}},      // Índice negativo.
        {{0, 2, 3}, {1}}        // Índice fora do grafo.
    };
    for (std::size_t i = 0; i < invalidas.size(); ++i)
        exigir_excecao<std::runtime_error>([&] { validar_coloracao(grafo, 3, invalidas[i]); },
                                          "Validador aceitou resultado incorreto " + std::to_string(i));
    validar_coloracao(grafo, 3, {{0, 2}, {}, {1}});
}
} // namespace

int main() {
    const std::vector<std::pair<const char*, void (*)()>> testes = {
        {"Grafo vazio", testar_vazio},
        {"Vertice isolado", testar_isolado},
        {"Grafo sem arestas", testar_sem_arestas},
        {"Grafo completo K6", testar_completo},
        {"Grafo bipartido K3,4", testar_bipartido},
        {"Ciclo impar", testar_ciclo_impar},
        {"Guloso pode usar mais cores que o minimo", testar_guloso_nao_minimo},
        {"Arestas duplicadas e remocoes", testar_duplicatas_e_remocao},
        {"Indices invalidos", testar_indices_invalidos},
        {"Construtor invalido", testar_construtor_invalido},
        {"Lacos e recuperacao apos excecao", testar_lacos},
        {"Arestas dirigidas nos dois sentidos", testar_arestas_dirigidas},
        {"Regressoes G1, G2 e G3", testar_exemplos},
        {"Impressao de vetor vazio", testar_impressao_vetor_vazio},
        {"Validador rejeita coloracoes incorretas", testar_validador}
    };
    int falhas = 0;
    for (const auto& teste : testes) {
        try {
            teste.second();
            std::cout << "[OK] " << teste.first << '\n';
        } catch (const std::exception& erro) {
            ++falhas;
            std::cerr << "[FALHA] " << teste.first << ": " << erro.what() << '\n';
        } catch (...) {
            ++falhas;
            std::cerr << "[FALHA] " << teste.first << ": excecao desconhecida\n";
        }
    }
    std::cout << testes.size() - falhas << "/" << testes.size() << " testes passaram\n";
    return falhas == 0 ? 0 : 1;
}
