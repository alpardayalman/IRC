#include "Server.hpp"

// Bu class mi what the hell, ama kb mergleme demistin dayanamadim. Buradan ilerleriz eline saglik xoxo. (Biseye benzet lutfen)
// Debug islerini Utilitieslere eklersen super olur.
// PASSwordu yazdiktan sonra sadece JOIN yazinca server dusuyor...

int  findChanel(std::string& name, std::vector<Chanel> chanels) {
    for (ChanelIterator it = chanels.begin(); it != chanels.end(); it++) {
        if (name == (*it).name)
            return 1;
    }
    return 0;
}

void showClients(Chanel &cha) {
    for (ClientIterator it = cha.clients.begin(); it != cha.clients.end(); it++) {
        std::cout << (*it).nick << "|";
    }
    std::cout << std::endl;
}

#define ShowUser
#define DebugJ

int    Server::Join(std::string &s, Client& cli) {
    std::stringstream ss(s);
    std::string chaName, key;
    ss >> chaName;
    ss >> key;
    chaName = chaName.substr(1, chaName.size());
#ifdef DebugJ
    std::cout << " chaName:"  << chaName << " key:" << key << '\n';
#endif
    if (findChanel(chaName, this->chanels)) {
        for (ChanelIterator it = chanels.begin(); it != chanels.end(); it++) {
            if (chaName == (*it).name)
                (*it).clients.push_back(cli);
        }
    } else {
        Chanel  newChanel(chaName);
        this->chanels.push_back(newChanel);
        this->chanels.back().clients.push_back(cli);
    }
#ifdef ShowUser
    showClients(chanels.back());
#endif
    return 0;
}