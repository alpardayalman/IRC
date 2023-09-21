#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>

// c++ receiver.cpp && ./a.out
int main() {
    const int port = 12345; // Alıcı port numarasını belirtin

    // Socket oluşturma
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Socket oluşturma hatası." << std::endl;
        return 1;
    }

    // Bağlantı ayarları
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Soketi bağlama
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Soketi bağlama hatası." << std::endl;
        close(sockfd);
        return 1;
    }

    // Bağlantı bekleniyor
    if (listen(sockfd, 1) == -1) {
        std::cerr << "Bağlantı bekleniyor hatası." << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << "Alıcı hazır, bağlantı bekleniyor..." << std::endl;

    // Bağlantı kabul etme
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSock = accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (clientSock == -1) {
        std::cerr << "Bağlantı kabul etme hatası." << std::endl;
        close(sockfd);
        return 1;
    }

    // Alınacak dosyanın adı
    std::string fileName = "received_file.txt";

    // Dosyayı oluştur ve verileri yaz
    std::ofstream outFile(fileName, std::ios::binary);
    if (!outFile) {
        std::cerr << "Dosya oluşturma hatası." << std::endl;
        close(clientSock);
        close(sockfd);
        return 1;
    }

    char buffer[1024];
    ssize_t bytesRead;
    while ((bytesRead = recv(clientSock, buffer, sizeof(buffer), 0)) > 0) {
        outFile.write(buffer, bytesRead);
    }

    if (bytesRead == -1) {
        std::cerr << "Veri alma hatası." << std::endl;
        outFile.close();
        close(clientSock);
        close(sockfd);
        return 1;
    }

    // Dosya ve soketleri kapat
    outFile.close();
    close(clientSock);
    close(sockfd);

    std::cout << "Dosya alındı: " << fileName << std::endl;

    return 0;
}
