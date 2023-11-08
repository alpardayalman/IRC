#include "Server.hpp"

void Server::createSocket( void ) {

    ((this->server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) ? 
    throw std::runtime_error( "Error: Socket can't be created.") : 
    std::cout << GREEN << "Success: Socket is created." << RESET << std::endl;



    this->is_running = true;
    
    (setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR, &this->reuse, sizeof(int)) < 0) ?
    throw std::runtime_error("Error: Optimizing socket.") :
    std::cout << GREEN << "Success: Socket is optimized." << RESET << std::endl;
}

void Server::serveraddrSocket( void ) const {
    struct sockaddr_in server_addr_socket;

    memset(&server_addr_socket, 0, sizeof(struct sockaddr_in));
    server_addr_socket.sin_family = AF_INET;
    server_addr_socket.sin_addr.s_addr = INADDR_ANY;
    server_addr_socket.sin_port = htons( this->port_number );

    (bind(this->server_fd, (struct sockaddr *) &server_addr_socket, sizeof(server_addr_socket))) < 0 ?
    throw std::runtime_error("Error: Socket is unbound.") :
    std::cout << GREEN << "Success: Socket is bound." << RESET << std::endl;

}

void    Server::socketListen( void ) const {

    (listen(this->server_fd, 128) < 0 )? //sysctl kern.ipc.somaxconn
    throw std::runtime_error("Error: Can't listen the server socket.") :
    std::cout << GREEN << "Success: Server socket is listening." << RESET << std::endl;
}