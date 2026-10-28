# Coloração de vértices

[![C++ CI](https://github.com/DedeDuarte/Grafo-Implementacao_Coloracao/actions/workflows/cpp.yml/badge.svg?branch=main)](https://github.com/DedeDuarte/Grafo-Implementacao_Coloracao/actions/workflows/cpp.yml)

Este projeto implementa a coloração sequencial de vértices em C++ usando a classe `TGrafo` e uma matriz de adjacência. O objetivo é atribuir cores aos vértices de modo que dois vértices adjacentes recebam cores diferentes.

Os exemplos acadêmicos representam grafos não direcionados: cada aresta é inserida nos dois sentidos com `insereA(v, w)` e `insereA(w, v)`.

## Como executar

É necessário ter um compilador com suporte a C++17 e o GNU Make instalados (por padrão, `g++`). Na raiz do projeto, execute:

```bash
make run
```

Esse comando compila e executa a demonstração acadêmica de `src/main.cpp`. O executável permanece disponível. Para apenas compilar e executar manualmente:

```bash
make
./a.out
```

Para remover os executáveis e artefatos gerados, mesmo quando ainda não existem:

```bash
make clean
```

Todas as compilações usam C++17 e os avisos `-Wall -Wextra -Wpedantic`. O compilador pode ser escolhido pela linha de comando, por exemplo:

```bash
make clean
make CXX=clang++
make test CXX=clang++
```

Ao mudar o compilador ou as flags, execute `make clean` antes de recompilar. Também é possível configurar `CPPFLAGS`, `CXXFLAGS`, `LDFLAGS` e `LDLIBS`; ao substituir `CXXFLAGS`, mantenha o padrão C++17 e os avisos desejados.

## Implementação da coloração

O método `TGrafo::coloracao()` percorre os vértices na ordem dos seus índices, de `0` a `n - 1`. Para cada vértice, tenta usar a primeira cor disponível, verificando se algum vértice já incluído nessa cor é seu vizinho. Se houver conflito, tenta a próxima cor.

O resultado é um `std::vector<std::vector<int>>`, em que cada vetor interno reúne os vértices de uma cor. São reservadas `n` classes, que podem incluir classes vazias; a quantidade de cores é a quantidade de classes não vazias. A função `imprimir_coloracao` exibe essas classes e o total de cores utilizadas. Para um grafo sem vértices, o resultado é vazio.

Por ser um algoritmo guloso, a quantidade de cores depende da ordem dos vértices e não é necessariamente a mínima para qualquer grafo. Nos três exemplos acadêmicos, a ordem utilizada permite alcançar o número mínimo esperado.

A classe armazena arcos dirigidos: inserir ou remover `v -> w` afeta apenas essa posição da matriz. Para a coloração própria, uma aresta em qualquer dos sentidos impede que os dois vértices compartilhem uma cor. A consulta `existeA(v, w) const` permite verificar a adjacência sem modificar o grafo.

O construtor aceita zero vértices e lança `std::invalid_argument` para quantidades negativas. Quantidades positivas estão sujeitas à memória disponível para a matriz de tamanho quadrático; falhas de alocação são propagadas, liberando as linhas já alocadas. Inserção, remoção e consulta lançam `std::out_of_range` para índices fora de `[0, n)`, sem alterar o grafo. Arestas duplicadas e remoções de arestas ausentes não alteram a contagem de arcos.

Laços podem ser inseridos e removidos, mas `coloracao()` lança `std::domain_error` enquanto existir algum laço, pois não há coloração própria nesse caso. Depois de remover os laços, o grafo pode ser colorido normalmente. Cópias e movimentos de `TGrafo` estão desabilitados para evitar compartilhamento acidental da matriz e dupla liberação.

## Testes

Os testes automatizados estão em `tests/test_coloracao.cpp`, separados da demonstração e sem dependências externas:

```bash
make test
```

São 15 grupos de testes que verificam cobertura de todos os vértices, ausência de repetições, índices válidos e cores diferentes para vértices adjacentes, independentemente da contagem de cores. Incluem grafo vazio, vértice isolado, grafo sem arestas, completo, bipartido, ciclo ímpar, duplicatas, remoções, índices inválidos, quantidades negativas, laços, arcos dirigidos, impressão de vetor vazio e regressões acadêmicas. Um grafo bipartido com ordem desfavorável demonstra que o guloso pode usar mais cores que o mínimo. Verificações em compilação impedem a reintrodução de cópia ou movimento superficial.

O próprio validador é testado com resultados incorretos de mesma quantidade de cores, incluindo vértices ausentes, duplicados, inválidos e adjacentes na mesma classe. Cada falha apresenta uma mensagem e faz o programa retornar código diferente de zero.

Os grafos G1, G2 e G3 são construídos pelos testes com a mesma função `preencher_grafos`, em `src/utils.cpp`, usada pela demonstração. Além da validade da coloração, os testes conferem simetria, ausência de laços, número de arestas e quantidade de cores:

| Teste | Vértices | Arestas não direcionadas | Número esperado de cores | Cores obtidas |
| :---- | :------: | :----------------------: | :----------------------: | :-----------: |
| G1    |     5    |            6             |             3            |       3       |
| G2    |     9    |            15            |             4            |       4       |
| G3    |    23    |            71            |             5            |       5       |

## Verificações de memória

Com GCC ou Clang com suporte a AddressSanitizer e UndefinedBehaviorSanitizer:

```bash
make sanitize
```

Esse alvo compila e executa os mesmos testes com `-g -fno-omit-frame-pointer -fsanitize=address,undefined`, detecção de vazamentos e interrupção em erros de ASan ou UBSan. Usa `build/test_coloracao_sanitize`, separado do executável normal `build/test_coloracao`.

Opcionalmente, instale o Valgrind (no Ubuntu: `sudo apt install valgrind`) e execute:

```bash
make valgrind
```

O alvo usa um executável próprio com símbolos de depuração, `build/test_coloracao_valgrind`, sem ASan, e as opções `--leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1`.

## GitHub Actions

O workflow [`.github/workflows/cpp.yml`](.github/workflows/cpp.yml) executa em pushes para `main`, pull requests destinados a `main` e manualmente pela aba Actions. O badge no início deste README consulta o status real desse workflow na branch `main`; ele passa a refletir as execuções quando o workflow estiver disponível no GitHub.

Os três jobs são independentes, executam em Ubuntu com GCC/g++ e possuem somente a permissão `contents: read`:

| Job | Verificação |
| :-- | :-- |
| Build | Compila com `make` e executa a demonstração com `make run`. |
| Tests | Compila e executa a suíte com `make test`. |
| Sanitizers | Compila e executa a suíte com ASan e UBSan por `make sanitize`. |

Falhas de compilação, testes ou sanitizers fazem o respectivo job falhar. O checkout utiliza a [ação oficial `actions/checkout`](https://github.com/actions/checkout/releases/tag/v7.0.1), na versão estável `v7`. O workflow não publica artefatos nem releases; Valgrind é uma verificação local opcional.

Para repetir a validação local completa:

```bash
make clean
make
make test
make sanitize
make run
make valgrind # opcional, requer Valgrind
make clean
```
