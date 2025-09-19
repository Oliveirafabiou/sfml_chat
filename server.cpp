#include <iostream>
#include <SFML/Network.hpp>

int main()
{
    sf::TcpListener listener;

    // bind the listener to a port
    if (listener.listen(53000) != sf::Socket::Status::Done)
    {
        // error...
        std::cerr << "Erro ao bindar a porta!" << std::endl;
        return -1; // Retorna um código de erro
    }
    else std::cout << "sucesso" << std::endl;

    // accept a new connection
    sf::TcpSocket client;
    if (listener.accept(client) != sf::Socket::Status::Done)
    {
        // error...
        std::cerr << "Erro ao aceitar a conexão!" << std::endl;
        return -1;
    }
    else {
        std::cout << "sucesso" << std::endl;
        std::string acceptText = "Conexão aceita do lado do cliente";
        client.send(acceptText.data(), acceptText.size());
    }

    //std::cout << "Conexão aceita do cliente: " << (std::string)client.getRemoteAddress() << std::endl;

    //std::cout << "Conexão aceita do cliente: " << client.getRemoteAddress().toString() << std::endl;

    // use "client" to communicate with the connected client,
    // and continue to accept new connections with the listener 

    return 0; // Retorno de sucesso
}