#include "../include/Utilities.hpp"

int Utilities::checkPortNumber(char *port_number){
    for (int i = 0; port_number[i]; i++) {
        if (!isdigit(port_number[i]))
            return 0;
    }
    return 1;
}
