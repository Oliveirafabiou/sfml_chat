#include "Servidor.hpp"
#include <iostream>
#include <thread>


int main() {
    Servidor servidor(2); // mínimo 2 jogadores
    if (!servidor.iniciar(54000)) return 1;

    while (true) {
        if (servidor.selector.wait(sf::seconds(1.f))) {
            servidor.aceitarConexoes();
            servidor.gerenciarMensagens();
        }
    }
}