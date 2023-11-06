#pragma once

#include "Server.hpp"

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