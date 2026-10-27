# Coloração de vértices

Este projeto implementa a coloração sequencial de vértices em C++ usando a classe `TGrafo` e uma matriz de adjacência. O objetivo é atribuir cores aos vértices de modo que dois vértices adjacentes recebam cores diferentes.

Os testes representam grafos não direcionados: cada aresta é inserida nos dois sentidos com `insereA(v, w)` e `insereA(w, v)`.

## Como executar

É necessário ter o `g++` com suporte a C++17 e o `make` instalados. Na raiz do projeto, execute:

```bash
make run
```

Esse comando compila o projeto, executa os testes e remove o executável ao final. Para apenas compilar e executar manualmente:

```bash
make
./a.out
```

Para remover o executável:

```bash
make clean
```

## Implementação da coloração

O método `TGrafo::coloracao()` percorre os vértices na ordem dos seus índices, de `0` a `n - 1`. Para cada vértice, tenta usar a primeira cor disponível, verificando se algum vértice já incluído nessa cor é seu vizinho. Se houver conflito, tenta a próxima cor.

O resultado é um `std::vector<std::vector<int>>`, em que cada vetor interno reúne os vértices de uma cor. A função `imprimir_coloracao` exibe as classes não vazias e o total de cores utilizadas.

Por ser um algoritmo guloso, a quantidade de cores pode depender da ordem dos vértices e não é necessariamente a mínima para qualquer grafo. Nos três testes atuais, a ordem utilizada permite alcançar o número mínimo esperado.

## Testes

Os grafos são construídos por `preencher_grafos`, em `src/utils.cpp`, e executados em `src/main.cpp`. O programa imprime as matrizes de adjacência e, em seguida, a coloração de cada grafo.

| Teste | Vértices | Arestas não direcionadas | Número esperado de cores | Cores obtidas |
| :---- | :------: | :----------------------: | :----------------------: | :-----------: |
| G1    |     5    |            6             |             3            |       3       |
| G2    |     9    |            15            |             4            |       4       |
| G3    |    23    |            71            |             5            |       5       |