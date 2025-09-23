#pragma once
#include <vector>
#include "Jogador.hpp"
#include "Baralho.hpp"

class Partida {
private:
    std::vector<Jogador*> jogadores;
    Baralho baralho;

public:
    Partida(std::vector<Jogador*> jogadores);
    void iniciar();
};