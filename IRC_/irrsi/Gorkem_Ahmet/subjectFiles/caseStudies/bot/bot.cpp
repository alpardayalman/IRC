#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main() {
    const char* server = "localhost";
    const int port = 1234;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Socket creation failed." << std::endl;
        return 1;
    }

    struct hostent* server_info = gethostbyname(server);
    if (!server_info) {
        std::cerr << "Host lookup failed." << std::endl;
        close(sock);
        return 1;
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    memcpy(&server_address.sin_addr.s_addr, server_info->h_addr, server_info->h_length);

    if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Connection failed." << std::endl;
        close(sock);
        return 1;
    }

    std::cout << "Connected to " << server << " on port " << port << std::endl;

    // Burada IRC sunucusuna mesaj gönderme ve mesajları alma işlemlerini ekleyebilirsiniz.

    close(sock);
    return 0;
}
