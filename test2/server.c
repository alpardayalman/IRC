#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    char *ip = "127.0.0.1";

    int port = 5566;

    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;

    socklen_t addr_size;

    char buffer[1024];
    int n;


    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("[-]Socket error");
        exit(EXIT_FAILURE);
    }
    printf("[+]TCP server socket created\n");

    memset(&server_addr, '\0', sizeof(server_addr));

    server_addr.sin_family = AF_INET; // internet
    server_addr.sin_port = port;// port
    server_addr.sin_addr.s_addr = inet_addr(ip); // local adress


    // bind ip adress and the port number 

    n = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (n < 0) {
        perror("[-]Bind error");
        exit(EXIT_FAILURE);
    }
    printf("[+]Bind to the port number: %d\n", port);


    // listen command

    // server is waiting for the client
    listen(server_sock, 5);
    printf("Listening...\n");

    while(1) {
        addr_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
        printf("[+]Client connected. \n");
    }

    return 0;
}