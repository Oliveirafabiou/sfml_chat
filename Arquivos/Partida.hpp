#pragma once
#include <vector>
#include <string>
#include <utility>
#include "Jogador.hpp"
#include "Baralho.hpp"
#include "Carta.hpp"

enum class EstadoTruco {
    NADA,
    TRUCO_PEDIDO,
    RETRUCO_PEDIDO, 
    VALE_QUATRO_PEDIDO, 
    ACEITO 
};

class Partida {
public:
    Partida(std::vector<Jogador*> jogadores);
    int iniciarNovaMao();
    std::string processarJogada(int jogadorIndex, int cartaIndex, int& proximoJogador);

    std::string processarPedidoTruco(int quemPediu, const std::string& comando, int& proximoJogador);
    std::string processarRespostaTruco(int quemRespondeu, const std::string& comando, int& proximoJogador, bool& maoEncerrada);

    EstadoTruco getEstadoTruco() const;
    int getJogadorEsperandoResposta() const;

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

    EstadoTruco estadoTruco;
    int pontosDaMao;
    int jogadorEsperandoResposta; 
    int turnoAntesDoTruco;

    void determinarVencedorDaRodada(int& proximoJogador);
    std::string concluirMao(int vencedorMao, int pontosGanhos, int& proximoJogador);
    int getVencedorDaMao();
};