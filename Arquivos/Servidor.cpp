#include "Servidor.hpp"
#include <iostream>

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

//void Servidor::aceitarConexoes() {
//    if (selector.isReady(listener)) {
//        auto* cliente = new sf::TcpSocket;
//        if (listener.accept(*cliente) == sf::Socket::Status::Done) {
//            clientes.push_back(cliente);
//            selector.add(*cliente);
//
//            int id = (int)clientes.size();
//            std::string nome = "Jogador" + std::to_string(id);
//            Jogador* j = new Jogador(id, nome);
//            jogadores.push_back(j);
//
//            std::cout << "Novo cliente conectado: " << nome << std::endl;
//            broadcast("Jogador " + nome + " entrou na sala.");
//
//            iniciarPartidaSePronto();
//        }
//        else {
//            delete cliente;
//        }
//    }
//}

void Servidor::aceitarConexoes() {
    // Verificamos se o listener tem alguma atividade.
    if (selector.isReady(listener)) {

        // ======================= LOG DE DEPURA플O =======================
        std::cout << "DEBUG: selector.isReady(listener) retornou true. Uma atividade foi detectada." << std::endl;

        auto* cliente = new sf::TcpSocket;

        // ======================= LOG DE DEPURA플O =======================
        std::cout << "DEBUG: Tentando chamar listener.accept(*cliente)..." << std::endl;

        sf::Socket::Status status = listener.accept(*cliente);

        // ======================= LOG DE DEPURA플O =======================
        // Vamos imprimir o status retornado, seja qual for.
        // (Done = 0, NotReady = 1, Partial = 2, Disconnected = 3, Error = 4)
        std::cout << "DEBUG: listener.accept() retornou o status: " << static_cast<int>(status) << std::endl;

        if (status == sf::Socket::Status::Done) {
            // ======================= LOG DE DEPURA플O =======================
            std::cout << "DEBUG: listener.accept() retornou SUCESSO (Done)." << std::endl;

            clientes.push_back(cliente);
            selector.add(*cliente);

            int id = (int)clientes.size();
            std::string nome = "Jogador" + std::to_string(id);
            Jogador* j = new Jogador(id, nome);
            jogadores.push_back(j);

            std::cout << ">>> SUCESSO: Novo cliente conectado: " << nome << std::endl;
            broadcast("Jogador " + nome + " entrou na sala.");

            iniciarPartidaSePronto();
        }
        else {
            // ======================= LOG DE DEPURA플O =======================
            std::cerr << "DEBUG: listener.accept() FALHOU. O status nao foi 'Done'." << std::endl;
            delete cliente;
        }
    }
}

void Servidor::gerenciarMensagens() {
    for (auto* c : clientes) {
        if (selector.isReady(*c)) {
            sf::Packet packet;
            if (c->receive(packet) == sf::Socket::Status::Done) {
                std::string msg;
                packet >> msg;
                std::cout << "Mensagem recebida: " << msg << std::endl;
                broadcast("Eco: " + msg);
            }
        }
    }
}

void Servidor::broadcast(const std::string& msg) {
    sf::Packet packet;
    packet << msg;
    for (auto* c : clientes) {
        c->send(packet);
    }
}

void Servidor::iniciarPartidaSePronto() {
    if ((int)jogadores.size() >= minJogadores && partida == nullptr) {
        std::cout << "Jogadores suficientes. Iniciando partida...\n";
        partida = new Partida(jogadores);
        partida->iniciar();

        // Enviar cartas para cada jogador
        for (size_t i = 0; i < jogadores.size(); i++) {
            sf::Packet p;
            p << "Suas cartas:\n" << jogadores[i]->maoToString();
            clientes[i]->send(p);
        }

        broadcast("Partida iniciada!");
    }
}