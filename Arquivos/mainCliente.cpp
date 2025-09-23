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

//int main() {
//    Cliente cliente("JogadorX");
//    if (!cliente.conectar("127.0.0.1", 54000)) return 1;
//
//    while (true) {
//        std::string entrada;
//        std::cout << "Digite mensagem: ";
//        std::getline(std::cin, entrada);
//        cliente.enviar(entrada);
//        cliente.ouvir();
//    }
//}


int main() {
    std::string nome;
    std::cout << "Digite seu nome: ";
    std::getline(std::cin, nome);

    Cliente cliente(nome);
    if (!cliente.conectar("127.0.0.1", 54000)) {
        return 1;
    }

    // Inicia a thread para ouvir constantemente as mensagens do servidor
    std::thread threadOuvinte(escutarServidor, std::ref(cliente));
    threadOuvinte.detach();

    // Loop principal para enviar mensagens com base no menu
    while (true) {
        std::string escolha;
        std::getline(std::cin, escolha); // A thread de escuta vai imprimir o menu "Escolha: "

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
            // Se o usuário digitar algo que não seja uma opção do menu,
            // podemos tratar como uma mensagem de chat ou simplesmente ignorar.
            // Por simplicidade, vamos ignorar.
            // O servidor já vai enviar o menu de novo quando for a vez do jogador.
        }
    }

    return 0; // O programa nunca chega aqui no loop atual
}