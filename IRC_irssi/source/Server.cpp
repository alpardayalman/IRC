#include "../include/Server.hpp"


Server::Server(size_t port_number, char * password) : port_number(port_number), password(std::string(password)) {
    std::cout << this->port_number << " " << this->password << std::endl;
}
Server::~Server() {
    std::cout << this->port_number << " " << this->password << std::endl;
}