---
marp: true
theme: default
paginate: true
style: |
  section { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; }
  h1 { color: #8B0000; } /* Cor da FEUP */
---

# Organização de Conferências Científicas
## Uma Abordagem com Network Flow
**Projeto I - Desenho de Algoritmos (FEUP)**

**Grupo:**
- *up202404210* (Domingos)
- *up202404098*
- *up202404099*

---

# Modelação com Fluxo Máximo (Network Flow)

[cite_start]Transformamos a atribuição num problema de Fluxo para garantir uma solução viável em tempo polinomial. [cite: 9, 27]

- **Super-Source ($S$):** Gera as necessidades de revisão.
- **Super-Sink ($T$):** Absorve a carga de trabalho completada.
- **Algoritmo Base:** **Edmonds-Karp**. [cite_start]Implementado com BFS para encontrar caminhos de aumento, garantindo complexidade $O(V \cdot E^2)$. [cite: 136]

---

# O Grafo - Estrutura e Capacidades

[cite_start]O nosso grafo é construído com base na estrutura das aulas de DA, com as seguintes capacidades: [cite: 126, 127]

1. [cite_start]**$S \rightarrow$ Submissões ($P_i$)**: Capacidade = `MinReviewsPerSubmission`. [cite: 24, 36]
2. **Submissões ($P_i$) $\rightarrow$ Revisores ($R_j$)**: Capacidade = $1$. [cite_start]Garante que um revisor avalia um artigo no máximo uma vez. [cite: 24]
3. [cite_start]**Revisores ($R_j$) $\rightarrow T$**: Capacidade = `MaxReviewsPerReviewer`. [cite: 24, 36]

[cite_start]**Nota de Implementação:** Usamos **Títulos** para artigos e **Emails** para revisores para garantir unicidade no grafo. [cite: 23, 39, 40]

---

# Arestas Intermédias: A Lógica do T2.4

[cite_start]A conectividade entre $P_i$ e $R_j$ segue os parâmetros de `GenerateAssignments`: [cite: 68]

- [cite_start]**Modo 1:** Apenas se `Primary Domain` == `Primary Expertise`. [cite: 71, 132]
- [cite_start]**Modo 2:** Se `Primary` ou `Secondary` do artigo coincidir com o `Primary` do revisor. [cite: 78]
- [cite_start]**Modo 3:** Match total entre quaisquer domínios (Primários ou Secundários) de ambas as partes. [cite: 79, 154]

---

# Risk Analysis $K=1$ (T2.2)

[cite_start]Em vez de uma análise estática, a nossa ferramenta realiza uma **Simulação de Falha**: [cite: 147]

- [cite_start]**Metodologia:** Para cada revisor, o sistema reconstrói o grafo excluindo-o e re-executa o Max-Flow. [cite: 82]
- [cite_start]**Validação:** Se o fluxo final for inferior ao necessário para cobrir todos os artigos, o revisor é marcado como **Crítico**. [cite: 84, 147]
- [cite_start]**Eficiência:** Uso de gestão de memória rigorosa (`delete g`) para permitir múltiplas simulações sem fugas de memória. [cite: 125]

---

# Abordagem Teórica: Risk Analysis ($K > 1$)

[cite_start]**Objetivo (T2.3):** Verificar se o sistema suporta a falha simultânea de $K$ revisores. [cite: 85, 150]

- [cite_start]**Estratégia:** Testar todas as combinações $\binom{R}{K}$ de revisores ausentes. [cite: 151]
- **Complexidade:** $O\left(\binom{R}{K} \times (V \cdot E^2)\right)$. [cite_start]Cresce exponencialmente, mas garante exaustividade total. [cite: 152]
- [cite_start]**Exemplo:** Num cenário com 10 revisores e $K=2$, testamos 45 variações do grafo para garantir a robustez da conferência. [cite: 152]

---

# Arquitetura e Implementação (C++)

- [cite_start]**Extensão da Classe Graph:** Adição de atributos `flow` e `reverse` na classe `Edge` para suporte a grafos residuais. [cite: 128]
- [cite_start]**Parsing:** Leitura robusta de CSVs ignorando comentários (#) e tratando duplicados com `std::set`. [cite: 38, 44]
- [cite_start]**Interface:** Menu interativo para visualização e Modo Batch (`-b`) para execução via script/correção automática. [cite: 116, 120, 121]

---

# Análise de Complexidade

| Tarefa | Algoritmo | Complexidade |
| :--- | :--- | :--- |
| **Parsing** | Linear | [cite_start]$O(S + R)$ [cite: 130] |
| **Max Flow** | Edmonds-Karp | [cite_start]$O(V \cdot E^2)$ [cite: 130, 148] |
| **Risk ($K=1$)** | Simulação Iterativa | [cite_start]$O(R \cdot (V \cdot E^2))$ [cite: 148] |
| **Risk ($K>K$)** | Combinatória | [cite_start]$O\left(\binom{R}{K} \cdot (V \cdot E^2)\right)$ [cite: 152] |

---

# Conclusão

- [cite_start]A modelação com **Network Flow** permite uma distribuição eficiente e justa da carga de revisão. [cite: 136]
- [cite_start]A nossa implementação garante que todos os requisitos de domínio e capacidade são satisfeitos. [cite: 161]
- [cite_start]A ferramenta de **Análise de Risco** é vital para garantir que a conferência não colapse perante imprevistos. [cite: 28, 134]