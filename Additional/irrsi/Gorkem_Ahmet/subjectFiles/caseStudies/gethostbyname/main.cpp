#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>

int main() {
    //const char* host_name = "www.google.com";
    const char* host_name = "127.0.0.1";
    hostent* host_info = gethostbyname(host_name);

    if (host_info) {
        in_addr** addr_list = reinterpret_cast<in_addr**>(host_info->h_addr_list);
        for (int i = 0; addr_list[i] != NULL; ++i) {
            std::cout << "IP Address " << i + 1 << ": " << inet_ntoa(*addr_list[i]) << std::endl;
        }
std::cout << "Client hostname: " << host_info->h_name << std::endl;

    } else {
        std::cerr << "Couldn't resolve the hostname." << std::endl;
    }

    return 0;
}
