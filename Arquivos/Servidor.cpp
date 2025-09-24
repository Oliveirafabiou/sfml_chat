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
    for (int i = (int)clientes.size() - 1; i >= 0; i--) {
        if (selector.isReady(*clientes[i])) {
            sf::Packet packet;
            sf::Socket::Status status = clientes[i]->receive(packet);

            if (status == sf::Socket::Status::Done) {
                std::string msg;
                packet >> msg;
                std::cout << "Mensagem de " << jogadores[i]->getNome() << ": " << msg << std::endl;

                if (msg.rfind("MSG ", 0) == 0) {
                    broadcast(jogadores[i]->getNome() + ": " + msg.substr(4));
                    continue;
                }

                if (partida == nullptr) continue;

                if (i != jogadorDaVezIndex) {
                    enviarMensagemPrivada(clientes[i], "[ERRO] Espere sua vez para jogar!");
                    continue;
                }

                std::stringstream ss(msg);
                std::string comando;
                ss >> comando;

                std::string broadcastMsg;

                if (comando == "JOGAR") {
                    int indiceCarta;
                    ss >> indiceCarta;
                    broadcastMsg = partida->processarJogada(i, indiceCarta, jogadorDaVezIndex);
                }
                else if (comando == "TRUCO") {
                    // Aqui você chamaria partida->processarTruco(i, jogadorDaVezIndex);
                    broadcastMsg = "!!!! " + jogadores[i]->getNome() + " PEDIU TRUCO !!!! (Logica a ser implementada)";
                    proximoJogador(); // Temporário
                }
                else {
                    enviarMensagemPrivada(clientes[i], "[ERRO] Comando desconhecido.");
                    continue;
                }

                broadcast(broadcastMsg);
                anunciarTurno();

            }
            else if (status == sf::Socket::Status::Disconnected) {

                std::string nomeDesconectado = jogadores[i]->getNome();
                std::cout << ">>> Jogador " << nomeDesconectado << " desconectado." << std::endl;

                selector.remove(*clientes[i]);

                delete clientes[i];
                delete jogadores[i];

                clientes.erase(clientes.begin() + i);
                jogadores.erase(jogadores.begin() + i);

                broadcast("O jogador " + nomeDesconectado + " saiu da partida.");

                if (partida != nullptr) {
                    if ((int)jogadores.size() < minJogadores) {
                        broadcast("Fim de jogo. Nao ha jogadores suficientes.");
                        delete partida;
                        partida = nullptr;
                    }
                    else {
                        if (jogadorDaVezIndex >= (int)jogadores.size()) {
                            jogadorDaVezIndex = 0;
                        }
                        anunciarTurno();
                    }
                }
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

void Servidor::anunciarTurno() {
    if (partida == nullptr || jogadores.empty()) return;

    std::string msgTurno = "Agora e a vez de " + jogadores[jogadorDaVezIndex]->getNome();
    broadcast(msgTurno);

    std::string menu = "\nSUA VEZ!\n[1] Jogar carta\n[2] Pedir truco\n[3] Enviar mensagem\nEscolha: ";
    enviarMensagemPrivada(clientes[jogadorDaVezIndex], menu);
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

        broadcast("Partida iniciada!");

        jogadorDaVezIndex = partida->iniciarNovaMao();

        for (size_t i = 0; i < jogadores.size(); i++) {
            std::string msg_cartas = "Suas cartas:\n" + jogadores[i]->maoToString();
            enviarMensagemPrivada(clientes[i], msg_cartas);
        }

        anunciarTurno();
    }
}