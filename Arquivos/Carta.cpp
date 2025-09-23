#include "Carta.hpp"

Carta::Carta(std::string naipe, int valor)
    : naipe(naipe), valor(valor) {
}

std::string Carta::toString() const {
    return std::to_string(valor) + " de " + naipe;
}

int Carta::getValor() const {
    return valor;
}

std::string Carta::getNaipe() const {
    return naipe;
}