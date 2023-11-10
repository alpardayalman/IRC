#pragma once

#include "Server.hpp"
//privmsg <chanelname> :<msg>
//chanelin msgboxina msg atabiliriz
#include "Client.hpp"

#define L 1
#define KEY 2


class Client;
class Chanel {
    private:

    public:
        std::vector<Client> clients;
        std::string         name;
        Client&             op;
        std::string         topic;
        std::string         key;

        int                 keycode;
        int                 users;

        
        Chanel(std::string &name, Client& cli): name(name), op(cli) {}
        ~Chanel() {}

        std::vector<int>    getFds() const;
};