#include "Baralho.hpp"
#include <algorithm>
#include <random>
#include <chrono>

Baralho::Baralho() {
    std::vector<std::string> naipes = { "Ouros", "Espadas", "Copas", "Paus" };
    for (auto& n : naipes) {
        for (int v = 1; v <= 12; v++) {
			if (v == 8 || v == 9) continue; // Pular 8 e 9
            cartas.emplace_back(n, v);
        }
    }
    embaralhar();
}

void Baralho::embaralhar() {
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cartas.begin(), cartas.end(), std::default_random_engine(seed));
}

Carta Baralho::puxarCarta() {
    if (cartas.empty()) throw std::runtime_error("Baralho vazio!");
    Carta c = cartas.back();
    cartas.pop_back();
    return c;
}

bool Baralho::vazio() const {
    return cartas.empty();
}
