#pragma once
#include <SFML/Network.hpp>
#include <string>

class Cliente {
private:
    sf::TcpSocket socket;
    std::string nome;

public:
    Cliente(std::string nome);
    bool conectar(const std::string& ip, unsigned short porta);
    void enviar(const std::string& msg);
    void ouvir();
};