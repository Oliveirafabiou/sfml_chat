#include "Cliente.hpp"
#include <iostream>

int main() {
    Cliente cliente("JogadorX");
    if (!cliente.conectar("192.168.100.26", 54000)) return 1;

    while (true) {
        std::string entrada;
        std::cout << "Digite mensagem: ";
        std::getline(std::cin, entrada);
        cliente.enviar(entrada);
        cliente.ouvir();
    }
}
