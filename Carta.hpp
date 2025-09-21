#pragma once
#include <string>

class Carta {
private:
	std::string naipe;
	int valor;

public:
	//Construtor
	Carta(std::string naipe, int valor);
	//Métodos
	// Retorna uma representação em string da carta
	std::string toString() const;
	int getValor() const;
	std::string getNaipe() const;
};