#include "Partida.hpp"
#include <iostream>
#include <algorithm>

Partida::Partida(std::vector<Jogador*> jogadores) : jogadores(jogadores) {
    pontosEquipe1 = 0;
    pontosEquipe2 = 0;
    dealerIndex = -1;
}

int Partida::iniciarNovaMao() {
    std::cout << "Iniciando nova mao...\n";
    baralho = Baralho(); 

    for (auto* j : jogadores) {
        j->limparMao();
        for (int i = 0; i < 3; i++) {
            j->receberCarta(baralho.puxarCarta());
        }
    }

    dealerIndex = (dealerIndex + 1) % jogadores.size();
    maoIndex = (dealerIndex + 1) % jogadores.size();

    rodadaAtual = 1;
    vencedorRodada[0] = vencedorRodada[1] = vencedorRodada[2] = -1;
    cartasNaMesa.clear();

    return maoIndex; 
}

std::string Partida::processarJogada(int jogadorIndex, int cartaIndex, int& proximoJogador) {
    try {
        Carta c = jogadores[jogadorIndex]->jogarCarta(cartaIndex);
        cartasNaMesa.push_back({ jogadorIndex, c });

        std::string broadcastMsg = jogadores[jogadorIndex]->getNome() + " jogou a carta: " + c.toString();

        if (cartasNaMesa.size() == jogadores.size()) {
            determinarVencedorDaRodada(proximoJogador);

            int vencedorMao = getVencedorDaMao();
            if (vencedorMao != -1) {
                broadcastMsg += "\n" + concluirMao(proximoJogador);
            }
            else { 
                broadcastMsg += "\n" + jogadores[vencedorRodada[rodadaAtual - 1]]->getNome() + " venceu a rodada! Proxima rodada...";
                rodadaAtual++;
                cartasNaMesa.clear();
            }
        }
        else {
            proximoJogador = (jogadorIndex + 1) % jogadores.size();
        }
        return broadcastMsg;

    }
    catch (const std::out_of_range& e) {
        return "[ERRO] Jogada invalida.";
    }
}

void Partida::determinarVencedorDaRodada(int& proximoJogador) {
    // Lógica para 2 jogadores
    auto jogada1 = cartasNaMesa[0];
    auto jogada2 = cartasNaMesa[1];

    if (jogada1.second.getForca() > jogada2.second.getForca()) {
        vencedorRodada[rodadaAtual - 1] = jogada1.first;
    }
    else if (jogada2.second.getForca() > jogada1.second.getForca()) {
        vencedorRodada[rodadaAtual - 1] = jogada2.first;
    }
    else {
        vencedorRodada[rodadaAtual - 1] = -2; // -2 = Empate (parda)
    }

    // Quem ganhou a rodada começa a próxima
    if (vencedorRodada[rodadaAtual - 1] != -2) {
        proximoJogador = vencedorRodada[rodadaAtual - 1];
    }
    else {
        proximoJogador = maoIndex; // Se empatou, o "mão" começa
    }
}

int Partida::getVencedorDaMao() {
    // Lógica para 2 jogadores (índices 0 e 1)
    int vitoriasP0 = 0;
    int vitoriasP1 = 0;

    for (int i = 0; i < 3; ++i) {
        if (vencedorRodada[i] == 0) vitoriasP0++;
        else if (vencedorRodada[i] == 1) vitoriasP1++;
    }

    if (vitoriasP0 >= 2) return 0;
    if (vitoriasP1 >= 2) return 1;

    // Regra do empate: primeira rodada empatou, quem ganhar a segunda vence.
    if (vencedorRodada[0] == -2) {
        if (vencedorRodada[1] == 0) return 0;
        if (vencedorRodada[1] == 1) return 1;
    }

    // Regra do empate: se a segunda empatar, quem ganhou a primeira vence.
    if (vencedorRodada[1] == -2) {
        if (vencedorRodada[0] == 0) return 0;
        if (vencedorRodada[0] == 1) return 1;
    }

    // Regra do empate: Se as duas primeiras empatam, quem vencer a terceira vence
    if (vencedorRodada[0] == -2 && vencedorRodada[1] == -2) {
        if (vencedorRodada[2] == 0) return 0;
        if (vencedorRodada[2] == 1) return 1;
    }

    // Se todas as 3 rodadas já foram jogadas e ninguém tem 2 vitórias (ex: V-D-E)
    if (rodadaAtual >= 3) {
        if (vitoriasP0 > vitoriasP1) return 0;
        if (vitoriasP1 > vitoriasP0) return 1;
    }

    // Empate total (3 empates, ou V-D e a 3a empata), o "mão" vence
    if ((vencedorRodada[0] != -1 && vencedorRodada[1] != -1 && vencedorRodada[2] != -1) ||
        (vencedorRodada[0] == -2 && vencedorRodada[1] == -2 && vencedorRodada[2] == -2)) {
        return maoIndex;
    }


    return -1; // Mão ainda não terminou
}

std::string Partida::concluirMao(int& proximoJogador) {
    int vencedor = getVencedorDaMao();
    int pontosGanhos = 1; // Simplificado: sempre vale 1 ponto. Lógica do Truco viria aqui.

    if (vencedor == 0) {
        pontosEquipe1 += pontosGanhos;
    }
    else {
        pontosEquipe2 += pontosGanhos;
    }

    std::string msg = ">> " + jogadores[vencedor]->getNome() + " venceu a mao e ganhou " + std::to_string(pontosGanhos) + " ponto(s)!";
    msg += "\n>> PLACAR: " + jogadores[0]->getNome() + " " + std::to_string(pontosEquipe1) + " x " + std::to_string(pontosEquipe2) + " " + jogadores[1]->getNome();

    if (pontosEquipe1 >= 30 || pontosEquipe2 >= 30) {
        msg += "\nFIM DE JOGO! " + jogadores[vencedor]->getNome() + " VENCEU A PARTIDA!";
    }
    else {
        // Prepara para a próxima mão
        proximoJogador = (dealerIndex + 1) % jogadores.size();
    }

    return msg;
}