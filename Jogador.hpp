#pragma once
#include <string>
#include <vector>
#include "Carta.hpp"

class Jogador {
private:
    int id;
    std::string nome;
    std::vector<Carta> mao;

public:
    Jogador(int id, std::string nome);
    void receberCarta(const Carta& c);
    Carta jogarCarta(int indice);
    std::string maoToString() const;
    std::string getNome() const;
    int getId() const;
};