#include "Chanel.hpp"

std::vector<int>    Chanel::getFds() const {
    std::vector<int> ret;
    for(int i = 0; i < (int) this->clients.size(); i++) {
        ret.push_back(this->clients[i].cliFd);
    }
    return(ret);
}
