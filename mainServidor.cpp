#include "Servidor.hpp"

int main() {
    Servidor servidor(2); // mínimo 2 jogadores
    if (!servidor.iniciar(54000)) return 1;

    while (true) {
        servidor.aceitarConexoes();
        servidor.gerenciarMensagens();
    }
}