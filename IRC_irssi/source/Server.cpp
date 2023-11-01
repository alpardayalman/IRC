#include "../include/Server.hpp"

Server::Server(size_t port_number, char * password) : port_number(port_number), password(std::string(password)) , reuse(1) {
    std::cout << this->port_number << " " << this->password << std::endl;
    createSocket();
    serveraddrSocket();
    socketListen();
    run();
}
Server::~Server() {
    if (this->is_running)
        close(this->server_fd);
    std::cout << "Server is shuting down." << std::endl;
}

/*
** Socket creation.
*/

void Server::createSocket( void ) {

    ((this->server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) ? 
    throw std::runtime_error( "Error: Socket can't be created.") : 
    std::cout << "Success: Socket is created." << std::endl;

    this->is_running = true;
    
    (setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR, &this->reuse, sizeof(int)) < 0) ?
    throw std::runtime_error("Error: Optimizing socket.") :
    std::cout << "Success: Socket is optimized." << std::endl;
}

void Server::serveraddrSocket( void ) const {
    struct sockaddr_in server_addr_socket;

    memset(&server_addr_socket, 0, sizeof(struct sockaddr_in));
    server_addr_socket.sin_family = AF_INET;
    server_addr_socket.sin_addr.s_addr = INADDR_ANY;
    server_addr_socket.sin_port = htons( this->port_number );

    (bind(this->server_fd, (struct sockaddr *) &server_addr_socket, sizeof(server_addr_socket))) < 0 ?
    throw std::runtime_error("Error: Socket is unbound.") :
    std::cout << "Success: Socket is bound." << std::endl;
}

void    Server::socketListen( void ) const {

    (listen(this->server_fd, 128) < 0 )? //sysctl kern.ipc.somaxconn
    throw std::runtime_error("Error: Can't listen the server socket.") :
    std::cout << "Success: Server socket is listening." << std::endl;
}


/*
 * Server Multiplexing
*/

void    Server::run( void ) {
    sockaddr_in cliAddr;
    socklen_t cliSize;
    int readed;
    char buffer[1024];
    Client tmp;
    cliSize = sizeof(sockaddr_in);
    int state = 0;

    FD_ZERO(&this->readFds);
    FD_ZERO(&this->writeFds);
    FD_ZERO(&this->readFdsSup);
    FD_ZERO(&this->writeFdsSup);

    FD_SET(this->server_fd, &this->readFds);
    int k = -1;
    while (1)
    {
        while (state == 0)
        {
            this->readFdsSup = this->readFds;
            this->writeFdsSup = this->writeFds;
            state = select(this->clients.size() + 4, &this->readFdsSup, &this->writeFdsSup, NULL, 0);
            std::cout << ++k << " ";
        }

        if (FD_ISSET(this->server_fd, &this->readFdsSup))
        {
            tmp.cliFd = accept(this->server_fd, (sockaddr *)&cliAddr, &cliSize);
            tmp.port = ntohs(cliAddr.sin_port);
            std::cout << "Top G:" << inet_ntop(AF_INET, &(cliAddr.sin_addr), tmp.ipAddr, INET_ADDRSTRLEN) << std::endl; // insanlar okuyabilsin diye.
            this->clients.push_back(tmp);
            FD_SET(tmp.cliFd, &this->readFds);
            std::cout << "New Client Connected!" << std::endl;
            write(tmp.cliFd, "Provide the password with this format: /PASS <password>\n", 57);
            state = 0;
        }

        // read eventinde.
        for(std::vector<Client>::iterator begin = this->clients.begin(); begin != this->clients.end() && state;++begin)
        {
            if (FD_ISSET((*begin).cliFd, &this->readFdsSup))
            {
                std::cout <<"Read" << std::endl;
                readed = read((*begin).cliFd, buffer, 1024);
                if (readed <= 0)
                {
                    FD_CLR((*begin).cliFd, &this->readFds);
                    FD_CLR((*begin).cliFd, &this->writeFds);
                    close((*begin).cliFd);
                    this->clients.erase(begin);
                    std::cout << "A client disconnected!" << std::endl;
                }
                else
                {
                    buffer[readed] = 0;

                    // BUFFER'i sondan ve bastan trim etmek gerekiyor --> somebody please.
                    // INFO
                    std::string s = buffer;

                    if (s.find("/INFO") != (unsigned long) -1) { // there is a newline at the end.
                        std::cout << "NICK:" <<(*begin).nick << " USER:" << (*begin).user << std::endl;
                    }

                    // PASSWORD
                    if ((*begin).passcheku)
                        std::cout << (*begin).cliFd << std::endl;
                    else if  (this->checkPassword(s, (*begin))) {
                        (*begin).passcheku = 1;
                        write((*begin).cliFd, "Password is corect\n", 20);
                        std::cout << "Client: " << (*begin).cliFd <<  " has the password correctly"<<std::endl;
                    }
                    else {
                        FD_CLR((*begin).cliFd, &this->readFds);
                        FD_CLR((*begin).cliFd, &this->writeFds);
                        write((*begin).cliFd, "Password is incorect\n", 22);
                        std::cout << "Client: " << (*begin).cliFd <<  " has the password incorrectly GTFO"<<std::endl;
                        close((*begin).cliFd);
                        this->clients.erase(begin);
                    }

                    if (s.find("/PRIVMSG") != (unsigned long) -1) {
                        for(std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end();++it)
                        {
                            if (it != begin)
                            {
                                (*it).messageBox.push_back(buffer);
                                FD_SET((*it).cliFd, &this->writeFds);
                            }
                        }
                    }
                }
                state = 0;
                break;
            }
        }



        //write eventinde.
        for(std::vector<Client>::iterator begin = this->clients.begin(); begin != this->clients.end() && state;++begin)
        {
            if (FD_ISSET((*begin).cliFd, &this->writeFdsSup))
            {
                std::cout <<" write" << std::endl;   
                readed = write((*begin).cliFd, (char *)(*begin).messageBox[0].c_str(), (*begin).messageBox[0].length());
                (*begin).messageBox.erase((*begin).messageBox.begin());

                if ((*begin).messageBox.empty())
                    FD_CLR((*begin).cliFd, &this->writeFds);

                if (readed <= 0)
                {
                    FD_CLR((*begin).cliFd, &this->readFds);
                    FD_CLR((*begin).cliFd, &this->writeFds);
                    close((*begin).cliFd); // BUNLA KAPATIYOZ CLIENTI maliz komple
                    this->clients.erase(begin);
                    std::cout << "A client disconnected!" << std::endl;
                }
                state = 0;
                break;
            }
        }
    }
}




bool    Server::checkPassword(std::string& s, Client& c) { // BUNU GUZELLESTIRELIM ALLAH ASKINA
    
    if (c.passcheku)
    {
        return 1;
    }
    else if (s.find("/PASS") != (unsigned long) -1) {
        int i = 5;
        for (;s[i] == 32;i++);
        for (int a = 0; this->password.c_str()[a]; a++) {// password make it better (Emircan)
            if (s[i + a] != this->password.c_str()[a])
                return 0;
        }
        return 1;
    }
    return 0;
}