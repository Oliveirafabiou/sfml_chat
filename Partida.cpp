#include "Partida.hpp"
#include <iostream>

Partida::Partida(std::vector<Jogador*> jogadores)
    : jogadores(jogadores), baralho() {
}

void Partida::iniciar() {
    std::cout << "Iniciando partida com " << jogadores.size() << " jogadores\n";

    for (auto* j : jogadores) {
        for (int i = 0; i < 3; i++) {
            j->receberCarta(baralho.puxarCarta());
        }
        std::cout << "Jogador " << j->getNome() << " recebeu:\n" << j->maoToString();
    }
}