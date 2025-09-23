#include "Servidor.hpp"
#include <iostream>
#include <thread>

//int main() {
//    Servidor servidor(2); // mínimo 2 jogadores
//    if (!servidor.iniciar(54000)) return 1;
//
//    int loopCount = 0;
//    while (true) {
//        servidor.aceitarConexoes();
//        servidor.gerenciarMensagens();
//
//
//        std::cout << "\rServidor rodando... " << loopCount++;
//        std::this_thread::sleep_for(std::chrono::milliseconds(50));
//    }
//}

int main() {
    Servidor servidor(2); // mínimo 2 jogadores
    if (!servidor.iniciar(54000)) return 1;

    // O loop principal DEVE usar selector.wait()
    while (true) {
        // Pausa e espera por atividade por até 1 segundo.
        // Se houver atividade, o if será verdadeiro.
        if (servidor.selector.wait(sf::seconds(1.f))) {
            servidor.aceitarConexoes();
            servidor.gerenciarMensagens();
        }
    }
}