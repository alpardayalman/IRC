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

void    Bot::writeRpl(int fd, std::string msg) {
    write(fd, msg.c_str(), msg.length());
}

std::vector<std::string> Bot::splitFromComa(const std::string& str) {

    std::vector<std::string> tokens;
    std::string token;
    
    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] != ',') {
            token += str[i];
        } else {
            tokens.push_back(token);
            token.clear();
        }
    }

    // Push the last token if any
    if (!token.empty()) {
        tokens.push_back(token);
    }

    return tokens;
}