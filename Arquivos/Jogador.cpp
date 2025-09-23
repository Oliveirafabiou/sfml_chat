#include "Jogador.hpp"

Jogador::Jogador(int id, std::string nome) : id(id), nome(nome) {}

void Jogador::receberCarta(const Carta& c) {
    mao.push_back(c);
}

Carta Jogador::jogarCarta(int indice) {
    if (indice < 0 || indice >= (int)mao.size()) {
		//throw std::out_of_range("Carta inválida!"); joga um erro caso a carta seja inválida ou nesse caso, joga a carta de índice zero:
		indice = 0;
    }
    Carta escolhida = mao[indice];
    mao.erase(mao.begin() + indice);
    return escolhida;
}

std::string Jogador::maoToString() const {
    std::string s;
    for (size_t i = 0; i < mao.size(); i++) {
        s += "[" + std::to_string((int)i) + "] " + mao[i].toString() + "\n";
    }
    return s;
}

std::string Jogador::getNome() const { return nome; }
int Jogador::getId() const { return id; }
