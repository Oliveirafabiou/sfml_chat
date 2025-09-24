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
                std::cout << "Mensagem crua de " << jogadores[i]->getNome() << ": \"" << msg << "\"" << std::endl;

                if (partida == nullptr) {
                    continue;
                }

                if (i != jogadorDaVezIndex) {
                    enviarMensagemPrivada(clientes[i], "[ERRO] Espere sua vez!");
                    continue;
                }

                std::stringstream ss(msg);
                int comandoNum;
                ss >> comandoNum;

                if (comandoNum == 4) {
                    std::string mao = "Sua mao atual:\n" + jogadores[i]->maoToString();
                    enviarMensagemPrivada(clientes[i], mao);
                    anunciarTurno(); 
                    continue;       
                }

                std::string broadcastMsg;
                bool maoEncerrada = false;


                if (partida->getJogadorEsperandoResposta() == i) {
                    std::string respostaCmd;
                    switch (comandoNum) {
                    case 1: respostaCmd = "ACEITO"; break;
                    case 2: respostaCmd = "NAO_QUERO"; break;
                    case 3: 
                        if (partida->getEstadoTruco() == EstadoTruco::TRUCO_PEDIDO) respostaCmd = "RETRUCO";
                        else if (partida->getEstadoTruco() == EstadoTruco::RETRUCO_PEDIDO) respostaCmd = "VALE_QUATRO";
                        else respostaCmd = "INVALIDO";
                        break;
                    default: respostaCmd = "INVALIDO"; break;
                    }

                    if (respostaCmd != "INVALIDO") {
                        broadcastMsg = partida->processarRespostaTruco(i, respostaCmd, jogadorDaVezIndex, maoEncerrada);
                    }
                    else {
                        enviarMensagemPrivada(clientes[i], "[ERRO] Opcao de resposta invalida.");
                        anunciarTurno(); 
                        continue;
                    }
                }

                else {
                    switch (comandoNum) {
                    case 1: { 
                        int indiceCarta;
                        ss >> indiceCarta;
                        if (ss.fail()) { 
                            enviarMensagemPrivada(clientes[i], "[ERRO] Formato invalido. Use: 1 <indice da carta>");
                            anunciarTurno();
                            continue;
                        }
                        else {
                            broadcastMsg = partida->processarJogada(i, indiceCarta, jogadorDaVezIndex);
                        }
                        break;
                    }
                    case 2: { 
                        broadcastMsg = partida->processarPedidoTruco(i, "TRUCO", jogadorDaVezIndex);
                        break;
                    }
                    case 3: { 
                        std::string corpoMsg;
                        std::getline(ss, corpoMsg);
                        if (!corpoMsg.empty() && corpoMsg[0] == ' ') {
                            corpoMsg = corpoMsg.substr(1);
                        }
                        broadcast(jogadores[i]->getNome() + ": " + corpoMsg);
                        anunciarTurno();
                        continue;
                    }
                    default:
                        enviarMensagemPrivada(clientes[i], "[ERRO] Comando desconhecido.");
                        anunciarTurno();
                        continue;
                    }
                }


                if (!broadcastMsg.empty()) {
                    broadcast(broadcastMsg);
                }

                if (maoEncerrada) {
                    jogadorDaVezIndex = partida->iniciarNovaMao();
                    for (size_t j = 0; j < jogadores.size(); j++) {
                        std::string msg_cartas = "\nNova mao! Suas cartas:\n" + jogadores[j]->maoToString();
                        enviarMensagemPrivada(clientes[j], msg_cartas);
                    }
                }

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
    if (jogadorDaVezIndex < 0 || jogadorDaVezIndex >= (int)jogadores.size()) {
        std::cerr << "ERRO: Indice de jogador da vez invalido!" << std::endl;
        return;
    }

    std::string msgTurno = "\n================================\nAgora e a vez de " + jogadores[jogadorDaVezIndex]->getNome();
    broadcast(msgTurno);

    std::string menu;
    if (partida->getJogadorEsperandoResposta() == jogadorDaVezIndex) {
        switch (partida->getEstadoTruco()) {
        case EstadoTruco::TRUCO_PEDIDO:
            menu = "\nVoce foi desafiado com TRUCO!\n[1] Aceitar\n[2] Nao Querer\n[3] Pedir RETRUCO\n[4] Ver minha mao\nEscolha: ";
            break;
        case EstadoTruco::RETRUCO_PEDIDO:
            menu = "\nVoce foi desafiado com RETRUCO!\n[1] Aceitar\n[2] Nao Querer\n[3] Pedir VALE QUATRO\n[4] Ver minha mao\nEscolha: ";
            break;
        case EstadoTruco::VALE_QUATRO_PEDIDO:
            menu = "\nVoce foi desafiado com VALE QUATRO!\n[1] Aceitar\n[2] Nao Querer\n[4] Ver minha mao\nEscolha: ";
            break;
        default:
            menu = "\nSUA VEZ!\n[1] Jogar carta (Ex: 1 0)\n[2] Pedir TRUCO\n[3] Enviar mensagem (Ex: 3 Ola)\n[4] Ver minha mao\nEscolha: ";
            break;
        }
    }
    else {
        menu = "\nSUA VEZ!\n[1] Jogar carta (Ex: 1 0)\n[2] Pedir TRUCO\n[3] Enviar mensagem (Ex: 3 Ola)\n[4] Ver minha mao\nEscolha: ";
    }
    enviarMensagemPrivada(clientes[jogadorDaVezIndex], menu);
}

void Servidor::proximoJogador() {
    jogadorDaVezIndex = (jogadorDaVezIndex + 1) % jogadores.size();
    std::string msgTurno = "Agora e a vez de " + jogadores[jogadorDaVezIndex]->getNome();
    broadcast(msgTurno);

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