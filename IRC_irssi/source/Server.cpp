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

void    Server::run( void ) {
    sockaddr_in cliAddr;
    socklen_t cliSize;
    int cliFd;
    int readed;
    char buffer[1024];
    char clientIP[INET_ADDRSTRLEN];
    int cliPort;
    Client tmp;

    cliSize = sizeof(sockaddr_in);
    cliFd = -1;
    int state = 0;


    fd_set readFds;
    fd_set writeFds;
    fd_set readFdsSup;
    fd_set writeFdsSup;

    FD_ZERO(&readFds);
    FD_ZERO(&writeFds);
    FD_ZERO(&readFdsSup);
    FD_ZERO(&writeFdsSup);

    FD_SET(this->server_fd, &readFds);
    int k = -1;
    while (1)
    {
        while (state == 0)
        {
            readFdsSup = readFds;
            writeFdsSup = writeFds;
            state = select(this->clients.size() + 4, &readFdsSup, &writeFdsSup, NULL, 0);
            std::cout << ++k << " ";
        }

        if (FD_ISSET(this->server_fd, &readFdsSup))
        {
            cliFd = accept(this->server_fd, (sockaddr *)&cliAddr, &cliSize);
            cliPort = ntohs(cliAddr.sin_port);
            std::cout << "Top G:" << inet_ntop(AF_INET, &(cliAddr.sin_addr), clientIP, INET_ADDRSTRLEN) << std::endl; // insanlar okuyabilsin diye.
            tmp.cliFd = cliFd;
            tmp.ipAddr = clientIP;
            tmp.port = cliPort;
            this->clients.push_back(tmp);
            FD_SET(cliFd, &readFds);
            std::cout << "New Client Connected!" << std::endl;
            write(cliFd, "Provide the password with this format: /PASS <password>\n", 57);
            state = 0;
        }

        // read eventinde.
        for(std::vector<Client>::iterator begin = this->clients.begin(); begin != this->clients.end() && state;++begin)
        {
            if (FD_ISSET((*begin).cliFd, &readFdsSup))
            {
                std::cout << k << " read" << std::endl;
                readed = read((*begin).cliFd, buffer, 1024);
                if (readed <= 0)
                {
                    FD_CLR((*begin).cliFd, &readFds);
                    FD_CLR((*begin).cliFd, &writeFds);
                    close((*begin).cliFd);
                    this->clients.erase(begin);
                    std::cout << "A client disconnected!" << std::endl;
                }
                else
                {
                    buffer[readed] = 0;

                    // INFO
                    if (!strcmp(buffer, "info\n")) { // there is a newline at the end.
                        std::cout << "NICK:" <<(*begin).nick << " USER:" << (*begin).user << std::endl;
                    }

                    // PASSWORD
                    std::string s = buffer;
                    if ((*begin).passcheku)
                        std::cout << (*begin).cliFd << std::endl;
                    else if  (this->AddUser_checkPassword(s, (*begin))) {
                        (*begin).passcheku = 1;
                        std::cout << "Client: " << (*begin).cliFd <<  " has the password correctly"<<std::endl;
                    }
                    else {
                        FD_CLR((*begin).cliFd, &readFdsSup);
                        FD_CLR((*begin).cliFd, &writeFdsSup);
                        write((*begin).cliFd, "Your password is incorect\n", 26);
                        std::cout << "Client: " << (*begin).cliFd <<  " has the password incorrectly GTFO"<<std::endl;
                        this->clients.erase(begin);
                    }

                    if (s.find("/PRIVMSG") != (unsigned long) -1) {
                        for(std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end();++it)
                        {
                            if (it != begin)
                            {
                                (*it).messageBox.push_back(buffer);
                                FD_SET((*it).cliFd, &writeFds);
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
            if (FD_ISSET((*begin).cliFd, &writeFdsSup))
            {
                std::cout << k << " write" << std::endl;   
                readed = write((*begin).cliFd, (char *)(*begin).messageBox[0].c_str(), (*begin).messageBox[0].length());
                (*begin).messageBox.erase((*begin).messageBox.begin());

                if ((*begin).messageBox.empty())
                    FD_CLR((*begin).cliFd, &writeFds);

                if (readed <= 0)
                {
                    FD_CLR((*begin).cliFd, &readFds);
                    FD_CLR((*begin).cliFd, &writeFds);
                    close((*begin).cliFd);
                    this->clients.erase(begin);
                    std::cout << "A client disconnected!" << std::endl;
                }
                state = 0;
                break;
            }
        }
    }
}




bool    Server::AddUser_checkPassword(std::string& s, Client& c) {
    
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