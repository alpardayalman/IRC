#include "Bot.hpp"




void   Bot::createSocket() {

    // Create socket
    ((this->sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) ? 
    throw Exception("Socket is not created.") : 
    std::cout << GREEN << "Success: Socket is created." << RESET << std::endl;

    (setsockopt(this->sockFd, SOL_SOCKET, SO_REUSEADDR, &this->reuse, sizeof(int)) < 0) ?
    throw Exception("Socket is not optimized.") :
    std::cout << GREEN << "Success: Socket is optimized." << RESET << std::endl;
}

void  Bot::serveraddrSocket() {

    memset(&this->addr_socket, 0, sizeof(struct sockaddr_in));
    this->addr_socket.sin_family = AF_INET;
    this->addr_socket.sin_port = htons( this->port_number );
    inet_pton(AF_INET, "127.0.0.1", &this->addr_socket.sin_addr);

    (bind(this->sockFd, (struct sockaddr *) &this->addr_socket, sizeof(this->addr_socket))) < 0 ?
    throw Exception("Socket is not bound.") :
    std::cout << GREEN << "Success: Socket is bound." << RESET << std::endl;
}

void Bot::socketConnect() {

    (connect(this->sockFd, reinterpret_cast<sockaddr*>(&this->addr_socket), sizeof(this->addr_socket)) < 0 ) ? //sysctl kern.ipc.somaxconn
    throw Exception("Socket is not connecting.") :
    std::cout << GREEN << "Success: Server socket is listening." << RESET << std::endl;
}