#include "Cliente.hpp"
#include <iostream>
#include <thread>
#include <limits>

void static escutarServidor(Cliente& cliente) {
    while (true) {
        cliente.ouvir();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}


int main() {
    std::string nome;
    std::cout << "Digite seu nome: ";
    std::getline(std::cin, nome);

    Cliente cliente(nome);
    if (!cliente.conectar("127.0.0.1", 54000)) {
        return 1;
    }

    std::thread threadOuvinte(escutarServidor, std::ref(cliente));
    threadOuvinte.detach();

    std::cout << "Conectado! Aguardando o inicio do jogo..." << std::endl;

    while (true) {
        std::string entrada;
        std::getline(std::cin, entrada);

        if (!entrada.empty()) {
            cliente.enviar(entrada);
        }
    }

    return 0;
}