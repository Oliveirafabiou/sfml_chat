#include "Servidor.hpp"
#include <iostream>
#include <sstream>

Servidor::Servidor(int minJogadores) : minJogadores(minJogadores) {}

bool Servidor::iniciar(unsigned short porta) {
    if (listener.listen(porta) != sf::Socket::Status::Done) {
        std::cerr << "Erro ao iniciar servidor!" << std::endl;
        return false;
    }
    selector.add(listener);
    std::cout << "Servidor escutando na porta " << porta << std::endl;
    return true;
}


void Servidor::aceitarConexoes() {
    if (selector.isReady(listener)) {
        auto* cliente = new sf::TcpSocket;
        if (listener.accept(*cliente) == sf::Socket::Status::Done) {
            clientes.push_back(cliente);
            selector.add(*cliente);

            int id = (int)clientes.size();
            std::string nome = "Jogador" + std::to_string(id);
            Jogador* j = new Jogador(id, nome);
            jogadores.push_back(j);

            std::cout << ">>> Novo cliente conectado: " << nome << std::endl;
            broadcast("Jogador " + nome + " entrou na sala.");

            iniciarPartidaSePronto();
        }
        else {
            delete cliente;
        }
    }
}

void Servidor::gerenciarMensagens() {
    for (size_t i = 0; i < clientes.size(); i++) {
        if (selector.isReady(*clientes[i])) {
            sf::Packet packet;
            sf::Socket::Status status = clientes[i]->receive(packet);

            if (status == sf::Socket::Status::Done) {
                std::string msg;
                packet >> msg;
                std::cout << "Mensagem de " << jogadores[i]->getNome() << ": " << msg << std::endl;

                std::stringstream ss(msg);
                std::string comando;
                ss >> comando; // Extrai o primeiro "token" (comando)

                // Mensagens de chat são permitidas a qualquer momento
                if (comando == "MSG") {
                    std::string corpoMsg;
                    std::getline(ss, corpoMsg); // Pega o resto da linha
                    broadcast(jogadores[i]->getNome() + ":" + corpoMsg);
                    continue;
                }

                // Para ações de jogo, verifica se é o turno do jogador
                if ((int)i != jogadorDaVezIndex) {
                    enviarMensagemPrivada(clientes[i], "[ERRO] Espere sua vez para jogar!");
                    continue;
                }

                // Processa comandos do jogador da vez
                if (comando == "JOGAR") {
                    int indiceCarta;
                    ss >> indiceCarta;
                    Carta c = jogadores[i]->jogarCarta(indiceCarta);
                    broadcast(jogadores[i]->getNome() + " jogou a carta: " + c.toString());

                    // Passa o token para o próximo jogador
                    proximoJogador();

                }
                else if (comando == "TRUCO") {
                    broadcast("!!!! " + jogadores[i]->getNome() + " PEDIU TRUCO !!!!");
                    // (Lógica do truco viria aqui)

                    // Por enquanto, apenas passa a vez
                    proximoJogador();

                }
                else {
                    enviarMensagemPrivada(clientes[i], "[ERRO] Comando desconhecido.");
                }

            }
            else if (status == sf::Socket::Status::Disconnected) {
                // (Lógica para tratar desconexão do jogador)
                std::cout << "Jogador " << jogadores[i]->getNome() << " desconectado." << std::endl;
            }
        }
    }
}

void Servidor::broadcast(const std::string& msg, sf::TcpSocket* aExcluir) {
    sf::Packet packet;
    packet << msg;
    for (auto* c : clientes) {
        if (c != aExcluir) {
            c->send(packet);
        }
    }
}

void Servidor::enviarMensagemPrivada(sf::TcpSocket* destinatario, const std::string& msg) {
    sf::Packet p;
    p << msg;
    destinatario->send(p);
}

void Servidor::proximoJogador() {
    jogadorDaVezIndex = (jogadorDaVezIndex + 1) % jogadores.size();
    std::string msgTurno = "Agora e a vez de " + jogadores[jogadorDaVezIndex]->getNome();
    broadcast(msgTurno);

    // Envia o menu apenas para o jogador da vez
    std::string menu = "\nSUA VEZ!\n[1] Jogar carta\n[2] Pedir truco\n[3] Enviar mensagem\nEscolha: ";
    enviarMensagemPrivada(clientes[jogadorDaVezIndex], menu);
}

void Servidor::iniciarPartidaSePronto() {
    if ((int)jogadores.size() >= minJogadores && partida == nullptr) {
        std::cout << "Jogadores suficientes. Iniciando partida...\n";
        partida = new Partida(jogadores);
        partida->iniciar();

        broadcast("Partida iniciada!");

        // Enviar cartas para cada jogador
        for (size_t i = 0; i < jogadores.size(); i++) {
            std::string msg_cartas = "Suas cartas:\n" + jogadores[i]->maoToString();
            enviarMensagemPrivada(clientes[i], msg_cartas);
        }

        // Inicia o primeiro turno
        jogadorDaVezIndex = 0; // Garante que o jogador 1 começa
        std::string msgTurno = "A partida comeca. E a vez de " + jogadores[jogadorDaVezIndex]->getNome();
        broadcast(msgTurno);

        // Envia o menu para o primeiro jogador
        std::string menu = "\nSUA VEZ!\n[1] Jogar carta\n[2] Pedir truco\n[3] Enviar mensagem\nEscolha: ";
        enviarMensagemPrivada(clientes[jogadorDaVezIndex], menu);
    }
}