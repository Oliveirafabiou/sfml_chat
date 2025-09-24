#pragma once
#include <string>

class Carta {
private:
	std::string naipe;
	int valor;
	int forca;

public:
	Carta(std::string naipe, int valor);
	std::string toString() const;
	int getValor() const;
	std::string getNaipe() const;
	int getForca() const;
	int getValorEnvido() const;
};