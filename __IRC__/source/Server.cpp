#include "../include/Server.hpp"
#include "../include/Utilities.hpp"

// #define debugCMD

void Server::initCommands( void ) {
    t_cmdFunct["PASS"] = &Server::Pass;
    t_cmdFunct["INFO"] = &Server::Info;
    t_cmdFunct["PRIVMSG"] = &Server::PrivMsg;
    t_cmdFunct["JOIN"] = &Server::Join;
    t_cmdFunct["NICK"] = &Server::Nick;
    t_cmdFunct["USER"] = &Server::User;
    t_cmdFunct["CAP"] = &Server::Cap;

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

void    Server::commandHandler(std::string parameters, Client& begin) { // what'sup with this function? Emir temize alabilecek misin por favor? Else absurt gozukuyor.
    std::vector<std::string> param = Utilities::tokenCmd(parameters);
    if (param.size() == 0)
        return;
    std::string cmd = param[0];
    param.erase(param.begin());
    if (t_cmdFunct.find(cmd) != t_cmdFunct.end()){
        (this->*t_cmdFunct[cmd])(param[0], begin);
    }
    else
        Utilities::fd_write_color(1, param[1]+"| Command is not found." , BLUE);
}


// * Server Multiplexing
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
            Utilities::fd_write_color(1, "New Client Connected!\n", GREEN);
            write(tmp.cliFd, "Welcome Please Provide Password\n", 33);
            state = 0;
        }

        // read eventinde.
        for(std::vector<Client>::iterator begin = this->clients.begin(); begin != this->clients.end() && state;++begin)
        {
            if (FD_ISSET((*begin).cliFd, &this->readFdsSup))
            {
                readed = read((*begin).cliFd, this->buffer, 1024);
                if (readed <= 0)
                {
                    FD_CLR((*begin).cliFd, &this->readFds);
                    FD_CLR((*begin).cliFd, &this->writeFds);
                    close((*begin).cliFd);
                    this->clients.erase(begin);
                    Utilities::fd_write_color(1, "A client disconnected!\n", RED);
                }
                else
                {
                    this->buffer[readed] = 0;

                    // Initial Tokenizer.
                    std::string k = this->buffer;
                    std::string s = Utilities::trim(k);
                    Utilities::fd_write_color(1, s+'\n', BLUE);
                    std::vector<std::string> parameters = Utilities::tokenNewline(s);
                    for (int i =0; i < (int)parameters.size(); i++) {
                        Server::commandHandler(parameters[i], (*begin));
                    }
                    if (!(Utilities::tokenCmd(parameters[0])[0] == "CAP")) {
                        if (!Pass(k, (*begin))) {
                            FD_CLR((*begin).cliFd, &this->readFds);
                            FD_CLR((*begin).cliFd, &this->writeFds);
                            write((*begin).cliFd, "Password is incorect\n", 22);
                            Utilities::fd_write_color(1,"Client: " + std::to_string((*begin).cliFd -3) + " has the password incorrectly GTFO\n", RED);

                            close((*begin).cliFd);
                            this->clients.erase(begin);
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
                readed = write((*begin).cliFd, (char *)(*begin).messageBox[0].c_str(), (*begin).messageBox[0].length());
                (*begin).messageBox.erase((*begin).messageBox.begin());

                if ((*begin).messageBox.empty())
                    FD_CLR((*begin).cliFd, &this->writeFds);

                if (readed <= 0)
                {
                    FD_CLR((*begin).cliFd, &this->readFds);
                    FD_CLR((*begin).cliFd, &this->writeFds);
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