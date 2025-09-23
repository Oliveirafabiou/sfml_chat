#pragma once
#include <SFML/Network.hpp>
#include <vector>
#include "Jogador.hpp"
#include "Partida.hpp"

class Servidor {
public:
    sf::TcpListener listener;
    sf::SocketSelector selector;
    std::vector<sf::TcpSocket*> clientes;
    std::vector<Jogador*> jogadores;
    Partida* partida = nullptr;
    int minJogadores;
    int jogadorDaVezIndex = 0;


    Servidor(int minJogadores = 2);
    bool iniciar(unsigned short porta);
    void aceitarConexoes();
    void gerenciarMensagens();
    void broadcast(const std::string& msg, sf::TcpSocket* aExcluir = nullptr);
    void enviarMensagemPrivada(sf::TcpSocket* destinatario, const std::string& msg);
    void proximoJogador();
    void iniciarPartidaSePronto();
};
