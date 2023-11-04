#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>

// example.txt dosyası oluşturmayı unutmayın.
// c++ sender.cpp && ./a.out
int main() {
    // Alıcı IP adresi ve port numarası
    const char* receiverIP = "127.0.0.1"; // Alıcı IP adresini belirtin
    const int receiverPort = 12345;       // Alıcı port numarasını belirtin

    // Gönderilecek dosyanın adı
    std::string fileName = "example.txt";

    // Dosya açma
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        std::cerr << "Dosya açma hatası." << std::endl;
        return 1;
    }

    // Socket oluşturma
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Socket oluşturma hatası." << std::endl;
        return 1;
    }

    // Bağlantı ayarları
    struct sockaddr_in destAddr;
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(receiverPort);
    inet_pton(AF_INET, receiverIP, &(destAddr.sin_addr));

    // Alıcıya bağlanma
    if (connect(sockfd, (struct sockaddr*)&destAddr, sizeof(destAddr)) == -1) {
        std::cerr << "Alıcıya bağlanma hatası." << std::endl;
        close(sockfd);
        return 1;
    }

    // Dosyayı oku ve gönder
    char buffer[1024];
    while (!file.eof()) {
        file.read(buffer, sizeof(buffer));
        ssize_t bytesRead = file.gcount();
        if (bytesRead <= 0) {
            break;
        }
        ssize_t bytesSent = send(sockfd, buffer, bytesRead, 0);
        if (bytesSent == -1) {
            std::cerr << "Dosya gönderme hatası." << std::endl;
            break;
        }
    }

    // Soket ve dosyayı kapat
    close(sockfd);
    file.close();

    std::cout << "Dosya gönderildi." << std::endl;

    return 0;
}
