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

    while (true) {
        std::string escolha;
        std::getline(std::cin, escolha);

        if (escolha == "1") {
            std::cout << "Digite o indice da carta: ";
            std::string indice;
            std::getline(std::cin, indice);
            cliente.enviar("JOGAR " + indice);
        }
        else if (escolha == "2") {
            cliente.enviar("TRUCO");
        }
        else if (escolha == "3") {
            std::cout << "Digite sua mensagem: ";
            std::string chat;
            std::getline(std::cin, chat);
            cliente.enviar("MSG " + chat);
        }
        else {
			std::cout << "Opcao invalida. Tente novamente.\n";
        }
    }

    return 0;
}