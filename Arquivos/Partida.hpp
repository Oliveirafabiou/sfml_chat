#pragma once
#include <vector>
#include <string>
#include <utility>
#include "Jogador.hpp"
#include "Baralho.hpp"
#include "Carta.hpp"

class Partida {
public:
    Partida(std::vector<Jogador*> jogadores);

    int iniciarNovaMao();

    std::string processarJogada(int jogadorIndex, int cartaIndex, int& proximoJogador);

private:
    std::vector<Jogador*> jogadores;
    Baralho baralho;

    int pontosEquipe1;
    int pontosEquipe2;


    int dealerIndex;  
    int maoIndex;     
    int rodadaAtual;  


    std::vector<std::pair<int, Carta>> cartasNaMesa;


    int vencedorRodada[3];


    void determinarVencedorDaRodada(int& proximoJogador);
    std::string concluirMao(int& proximoJogador);
    int getVencedorDaMao();
};