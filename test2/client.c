#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    char *ip = "127.0.0.1";
    int port = 5566;

    int sock;
    struct sockaddr_in addr;
    socklen_t addr_size;
    char buffer[1024];
    int n;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("[-]Socket error");
        exit(EXIT_FAILURE);
    }
    printf("[+]TCP server socket created\n");

    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET; // internet
    addr.sin_port = port;// port
    addr.sin_addr.s_addr = inet_addr(ip); // local adress

    // connect to the server

    connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    printf("Connected to the server.\n");

    return 0;
}