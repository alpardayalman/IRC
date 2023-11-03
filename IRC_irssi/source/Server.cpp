#include "../include/Server.hpp"
#include "../include/Utilities.hpp"

void Server::initCommands( void ) {
    t_cmdFunct["PASS"] = &Server::Pass;
    t_cmdFunct["INFO"] = &Server::Info;
    t_cmdFunct["PRIVMSG"] = &Server::PrivMsg;

}

Server::Server(size_t port_number, char * password) : port_number(port_number), password(std::string(password)) , reuse(1) {
    std::cout << this->port_number << " " << this->password << std::endl;
    createSocket();
    serveraddrSocket();
    socketListen();
    initCommands();
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

void    Server::commandHandler(std::vector<std::string> param, Client& begin) {
    if (param.size() == 0)
        return;
    std::string cmd = param[0];
    param.erase(param.begin());
    std::cout << "cmd: " << cmd << std::endl;
    std::cout << "param: " << param[0] << std::endl;
    if (t_cmdFunct.find(cmd) != t_cmdFunct.end()){
        std::cout << "Command is found." << std::endl;
        (this->*t_cmdFunct[cmd])(param[0], begin);
    }
    else
        std::cout << "Command is not found." << std::endl;
}

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

#define DEBUG_1
/*
 * Server Multiplexing
*/

void    Server::run( void ) {
    sockaddr_in cliAddr;
    socklen_t cliSize;
    int readed;
    Client tmp;
    cliSize = sizeof(sockaddr_in);
    int state = 0;

    FD_ZERO(&this->readFds);
    FD_ZERO(&this->writeFds);
    FD_ZERO(&this->readFdsSup);
    FD_ZERO(&this->writeFdsSup);

    FD_SET(this->server_fd, &this->readFds);

    while (1)
    {
        while (state == 0)
        {
            this->readFdsSup = this->readFds;
            this->writeFdsSup = this->writeFds;
            state = select(this->clients.size() + 4, &this->readFdsSup, &this->writeFdsSup, NULL, 0);
        }

        if (FD_ISSET(this->server_fd, &this->readFdsSup))
        {
            tmp.cliFd = accept(this->server_fd, (sockaddr *)&cliAddr, &cliSize);
            tmp.port = ntohs(cliAddr.sin_port);
            inet_ntop(AF_INET, &(cliAddr.sin_addr), tmp.ipAddr, INET_ADDRSTRLEN); // insanlar okuyabilsin diye.
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
#ifdef DEBUG
                std::cout <<"Read" << std::endl;
#endif
                readed = read((*begin).cliFd, this->buffer, 1024);
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
                    this->buffer[readed] = 0;
                    // INFO
                    // {
                    // asagida trim edilmis buffer'i s olarak tutuyoruz ve cmd fonksiyonlarimizi ona gore yaptik. 
                    // tokenleri
                    std::string k = this->buffer;
                    std::string s = Utilities::trim(k); // trimming the shit out of them.

                    // istream ve tokenleme.
                    std::istringstream iss(s);
                    std::string command;
                    std::vector<std::string> parameters;
                    int paramlen;
                    // Extract the command
                    // iss >> command;

                    // Extract parameters
                    std::string param;
                    iss >> param;
                    parameters.push_back(param);
                    paramlen = param.length();
                    s.erase(0, paramlen + 1);
                    parameters.push_back(s);
#ifdef DEBUG_1
                    for (int i = 0; i < (int)parameters.size(); i++) {
                        std::cout << i <<": " << parameters[i] << std::endl;
                    }
#endif

                    // }
                    // EMIRCAN: abi moduler olasun diye su bagsettigimiz gibi bir map guzel olur map['INFO'] = &Server::Info tarzi.
                    // Talha: mayali su chanel isinde ilerlersen cok hizlaniriz
                    // Emre: her client girdiginde atabilecegimiz bir assci art fonksiyonu olsa cok matrak olur.
                    Server::commandHandler(parameters, (*begin));
                    if (!Pass(k, (*begin))) {
                        FD_CLR((*begin).cliFd, &this->readFds);
                        FD_CLR((*begin).cliFd, &this->writeFds);
                        write((*begin).cliFd, "Password is incorect\n", 22);
                        std::cout << "Client: " << (*begin).cliFd <<  " has the password incorrectly GTFO"<<std::endl;
                        close((*begin).cliFd);
                        this->clients.erase(begin);
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
#ifdef DEBUG
                std::cout <<" write" << std::endl;   
#endif
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
