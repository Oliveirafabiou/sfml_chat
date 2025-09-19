//#include <iostream>
//#include <SFML/Network.hpp>
//
//int main()
//{
//    sf::TcpSocket socket;
//
//    sf::Socket::Status status = socket.connect({ 192, 168, 0, 5 }, 53000);
//
//    if (status != sf::Socket::Status::Done)
//    {
//        // error...
//        std::cerr << "Erro ao conectar-se ao servidor!" << std::endl;
//        return -1; // Retorna um código de erro
//    }
//
//    std::cout << "Conectado ao servidor!" << std::endl;
//
//    // Use o "socket" para se comunicar com o servidor
//
//    return 0; // Retorno de sucesso
//}