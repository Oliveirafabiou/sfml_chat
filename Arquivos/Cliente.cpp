#include "Cliente.hpp"
#include <iostream>

Cliente::Cliente(std::string nome) : nome(nome) {}

bool Cliente::conectar(const std::string& ip, unsigned short porta) {
    auto resolvedAddress = sf::IpAddress::resolve(ip);
	std::cout << "Tentando conectar ao servidor " << ip << ":" << porta << "...\n";

    if (socket.connect(*resolvedAddress, porta) != sf::Socket::Status::Done) {
        std::cerr << "Erro ao conectar ao servidor!\n";
        return false;
    }

    socket.setBlocking(false);
    std::cout << "Conectado ao servidor!\n";
    return true;
}

void Cliente::enviar(const std::string& msg) {
    sf::Packet packet;
    packet << msg;
    socket.send(packet);
}

void Cliente::ouvir() {
    sf::Packet packet;
    if (socket.receive(packet) == sf::Socket::Status::Done) {
        std::string msg;
        packet >> msg;
        std::cout << "\n[Servidor]: " << msg << std::endl << "Digite mensagem: ";
    }
}
