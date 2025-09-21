#pragma once
#include <string>

class Carta {
private:
	std::string naipe;
	int valor;

public:
	//Construtor
	Carta(std::string naipe, int valor);
	//M�todos
	// Retorna uma representa��o em string da carta
	std::string toString() const;
	int getValor() const;
	std::string getNaipe() const;
};