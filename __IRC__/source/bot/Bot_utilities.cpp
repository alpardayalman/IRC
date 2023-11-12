#include "Bot.hpp"

int Bot::checkPortNumber(char *port) {
    for (int i = 0; port[i]; i++) {
        if (!isdigit(port[i]))
            return 0;
    }
    return 1;
}

std::vector<std::string> Bot::splitString(const std::string& input) {
    std::istringstream iss(input);
    std::vector<std::string> result;

    do {
        std::string word;
        iss >> word;
        result.push_back(word);
    } while (iss);

    result.pop_back();

    return result;
}