#pragma once

#include "Server.hpp"

class Chanel {
    private:

    public:
        std::vector<Client> clients;
        std::string name;

        Chanel(std::string &name): name(name) {}
        ~Chanel() {}

};