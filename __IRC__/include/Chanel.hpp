#pragma once

#include "Server.hpp"
//privmsg <chanelname> :<msg>
//chanelin msgboxina msg atabiliriz

class Client;

class Chanel {
    private:

    public:
        //maybe map<CLIENT{or name}, ISADMIN>
        std::vector<Client> clients;
        std::vector<Client> admin;
        std::string name;

        Chanel(std::string &name): name(name) {}
        ~Chanel() {}

};