# Wiki do Jogo de Truco Online

Bem-vindo ao nosso jogo de Truco online! Este guia irá ensinar-lhe tudo o que você precisa saber, desde os comandos básicos até as regras detalhadas do Truco.

## 📜 Sumário
1.  [Como Jogar (O Tutorial Prático)](#como-jogar-o-tutorial-prático)
2.  [Regras do Truco (A Teoria)](#regras-do-truco-a-teoria)
    *   [Estrutura do Jogo](#estrutura-do-jogo)
    *   [Hierarquia das Cartas](#hierarquia-das-cartas)
    *   [As Rodadas (Mãos)](#as-rodadas-mãos)
    *   [As Apostas (Truco)](#as-apostas-truco)

---

## 🚀 Como Jogar (O Tutorial Prático)

Esta seção foca em como interagir com o nosso jogo através dos comandos no terminal.

### 1. Conectando e Começando

Para jogar, execute o cliente e conecte-se ao servidor. Assim que o número mínimo de jogadores entrar na sala, a partida começará automaticamente.

### 2. Recebendo suas Cartas

No início de cada mão, você receberá 3 cartas. Elas serão mostradas a você de forma privada, com um índice:

```
Suas cartas:
 7 de Ouros
 1 de Espadas
 4 de Paus
```
O **índice** (o número entre colchetes) é como você irá se referir à sua carta ao jogar.

### 3. Seu Turno: O Menu de Ações

Quando for a sua vez de jogar, você receberá um menu com as opções válidas para aquele momento.

**Exemplo de Menu de Jogo Normal:**
```
SUA VEZ!
 Jogar carta (Ex: 1 0)
 Pedir TRUCO
 Enviar mensagem (Ex: 3 Ola)
 Ver minha mao
Escolha:
```

### 4. Comandos Principais

Aqui está o que cada opção faz e como usá-la:

*   **[1] Jogar uma Carta**
    *   **Comando:** `1 <índice da carta>`
    *   **Exemplo:** Para jogar o seu "7 de Ouros" do exemplo acima (que tem o índice `0`), você digitaria: `1 0`
    *   **Ação:** A carta escolhida é jogada na mesa.

*   **[2] Pedir TRUCO**
    *   **Comando:** `2`
    *   **Ação:** Você desafia seu oponente, aumentando o valor da mão para 2 pontos. O jogo irá parar e esperar a resposta do seu adversário.

*   **[3] Enviar Mensagem**
    *   **Comando:** `3 <sua mensagem>`
    *   **Exemplo:** Para dizer "Boa sorte!", você digitaria: `3 Boa sorte!`
    *   **Ação:** Envia uma mensagem de chat para todos na partida.

*   **[4] Ver sua Mão**
    *   **Comando:** `4`
    *   **Ação:** Suas cartas atuais serão exibidas novamente para você. **Isso não gasta o seu turno!** Após ver suas cartas, você receberá o menu de ações novamente.

### 5. Respondendo a um Desafio (Truco)

Se o seu oponente pedir "TRUCO", o seu menu de opções irá mudar para refletir as respostas possíveis.

**Exemplo de Menu de Resposta ao TRUCO:**
```
Voce foi desafiado com TRUCO!
 Aceitar
 Nao Querer
 Pedir RETRUCO
Escolha:
```

*   **[1] Aceitar:**
    *   **Comando:** `1`
    *   **Ação:** A mão passa a valer os pontos da aposta (2 para Truco, 3 para Retruco, etc.) e o jogo continua.

*   **[2] Não Querer:**
    *   **Comando:** `2`
    *   **Ação:** Você desiste da mão. O seu oponente ganha os pontos da aposta anterior (se ele pediu Truco, ele ganha 1 ponto). Uma nova mão começará.

*   **[3] Aumentar a Aposta (Retruco / Vale Quatro):**
    *   **Comando:** `3`
    *   **Ação:** Você aumenta a aposta e joga o desafio de volta para o seu oponente.

---

## 📖 Regras do Truco (A Teoria)

Esta seção explica as regras do jogo em si.

### Estrutura do Jogo

*   **Objetivo:** Ser o primeiro jogador (ou dupla) a atingir **21 pontos**.
*   **Baralho:** Usa-se o baralho espanhol de 40 cartas (naipes: Ouros, Espadas, Copas e Paus), sem as cartas 8 e 9.
*   **Jogadores:** Esta versão está configurada para 2 jogadores.

### Hierarquia das Cartas

No Truco, as cartas têm uma força fixa que não segue a ordem numérica. A carta mais forte é o Ás de Espadas e a mais fraca é o 4. O naipe só importa para as cartas mais fortes (as "manilhas").

| Força (Rank) | Carta(s)                  | Apelido / Observação      |
| :----------: | :------------------------ | :------------------------ |
|      1       | **1 de Espadas**          | "Espadão" - A mais forte  |
|      2       | **1 de Paus**             | "Bastão"                  |
|      3       | **7 de Espadas**          |                           |
|      4       | **7 de Ouros**            |                           |
|      5       | Todos os **3**s           |                           |
|      6       | Todos os **2**s           |                           |
|      7       | **1 de Copas**, **1 de Ouros** | "Ás Falso"                |
|      8       | Todos os **12**s          | Reis                      |
|      9       | Todos os **11**s          | Cavalos                   |
|      10      | Todos os **10**s          | Valetes                   |
|      11      | **7 de Copas**, **7 de Paus** | "7s Falsos"               |
|      12      | Todos os **6**s           |                           |
|      13      | Todos os **5**s           |                           |
|      14      | Todos os **4**s           | A mais fraca              |

### As Rodadas (Mãos)

*   Cada "mão" vale 1 ponto (a menos que haja uma aposta de Truco).
*   Uma mão é disputada em um sistema de "melhor de 3". Quem jogar a carta mais forte em uma rodada, ganha essa rodada. Quem ganhar 2 de 3 rodadas, vence a mão inteira e leva os pontos.

#### Regras de Empate (`Parda`)
O empate é uma parte crucial do Truco:
*   **Empate na 1ª Rodada:** Quem vencer a 2ª rodada, vence a mão. Se a 2ª também empatar, quem vencer a 3ª leva a mão.
*   **Empate na 2ª Rodada:** Quem venceu a 1ª rodada, vence a mão.
*   **Empate na 3ª Rodada:** Quem venceu a 1ª rodada, vence a mão.
*   **Se todas as rodadas empatarem:** O jogador que começou a primeira rodada (o "mão") vence.

### As Apostas (Truco)

A qualquer momento do seu turno, você pode "cantar" um **TRUCO** para aumentar o valor da mão.

*   **1. TRUCO**
    *   O jogador 1 canta "TRUCO!". A mão, que valia 1 ponto, agora está valendo 2.
    *   O jogador 2 pode:
        *   **ACEITAR:** O jogo continua, valendo 2 pontos.
        *   **NÃO QUERER:** O jogador 1 ganha **1 ponto** e a mão acaba.
        *   **Aumentar para RETRUCO:** O desafio é jogado de volta.

*   **2. RETRUCO**
    *   O jogador 2 canta "RETRUCO!". A mão agora está valendo 3 pontos.
    *   O jogador 1 pode:
        *   **ACEITAR:** O jogo continua, valendo 3 pontos.
        *   **NÃO QUERER:** O jogador 2 ganha **2 pontos** e a mão acaba.
        *   **Aumentar para VALE QUATRO:** O desafio final.

*   **3. VALE QUATRO**
    *   O jogador 1 canta "VALE QUATRO!". A mão agora está valendo 4 pontos.
    *   O jogador 2 pode:
        *   **ACEITAR:** O jogo continua, valendo 4 pontos.
        *   **NÃO QUERER:** O jogador 1 ganha **3 pontos** e a mão acaba.

Boa sorte e divirta-se!