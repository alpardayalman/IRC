#include "Utilities.hpp"
#include "Server.hpp"
#include "Exception.hpp"
#include <stdlib.h>
#include <stdexcept>
#include <iostream>

#define SYSTEM

void    signalHandle(int signum) {
#ifdef SYSTEM
    system("leaks ircserv"); //
#endif
    std::exit(signum);
}

int main(int ac, char **av) {
    try {
        if (ac != 3)
            throw Exception("Give arguments like the following ./exec <port> <password>.");
        else if (!Utilities::checkPortNumber(av[1]))
            throw Exception("Port has to be all digits.");
        else {
            std::signal(SIGINT, signalHandle);
            Server a = Server(atoi(av[1]), av[2]);
        }
    }
    catch (std::exception &ex) {
        std::cerr << ex.what() << std::endl;
    }
    return 1;
}
