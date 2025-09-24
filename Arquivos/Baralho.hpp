#pragma once
#include <vector>
#include "Carta.hpp"

class Baralho {
private:
    std::vector<Carta> cartas;

public:
    Baralho();
    void embaralhar();
    Carta puxarCarta();
    bool vazio() const;
};