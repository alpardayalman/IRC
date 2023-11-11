#pragma once

#include "Server.hpp"
//privmsg <chanelname> :<msg>
//chanelin msgboxina msg atabiliriz
#include "Client.hpp"

#define L_CODE 1
#define K_CODE 2


class Client;
class Chanel {
    private:

    public:
        std::vector<Client> clients;
        std::string         name;
        Client*             op;
        std::string         topic;
        std::string         key;

        int                 keycode; // hangi key var.
        int                 users;

        
        Chanel(std::string &name): name(name) , keycode(0){}
        ~Chanel() {}

        std::vector<int>    getFds() const;
        Client  &getClient(std::string &name_);
};