#include "Carta.hpp"
#include <map> 

Carta::Carta(std::string naipe, int valor)
    : naipe(naipe), valor(valor) {
    // Mapa estático para definir a hierarquia (força) das cartas.
    // Quanto maior o valor, mais forte a carta.
    static const std::map<std::pair<int, std::string>, int> hierarquia = {
        // "Manilhas" (cartas mais fortes)
        {{1, "Espadas"}, 14},
        {{1, "Paus"},    13},
        {{7, "Espadas"}, 12},
        {{7, "Ouros"},   11},
        // Cartas comuns
        {{3, "Qualquer"}, 10},
        {{2, "Qualquer"}, 9},
        {{1, "Ouros"},    8},
        {{1, "Copas"},    8},
        {{12, "Qualquer"}, 7},
        {{11, "Qualquer"}, 6},
        {{10, "Qualquer"}, 5},
        {{7, "Copas"},    4},
        {{7, "Paus"},     4},
        {{6, "Qualquer"}, 3},
        {{5, "Qualquer"}, 2},
        {{4, "Qualquer"}, 1}
    };

    this->forca = 0;
    auto it_especifico = hierarquia.find({ this->valor, this->naipe });
    auto it_qualquer = hierarquia.find({ this->valor, "Qualquer" });

    if (it_especifico != hierarquia.end()) {
        this->forca = it_especifico->second;
    }
    else if (it_qualquer != hierarquia.end()) {
        this->forca = it_qualquer->second;
    }
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

int Carta::getForca() const {
    return forca;
}


int Carta::getValorEnvido() const {
    if (valor >= 10 && valor <= 12) {
        return 0; 
    }
    return valor;
}