#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>

// ICMP başlığı hesaplama işlevi
unsigned short calculateChecksum(unsigned short* buffer, int size) {
    unsigned long sum = 0;
    while (size > 1) {
        sum += *buffer++;
        size -= sizeof(unsigned short);
    }
    if (size) {
        sum += *(unsigned char*)buffer;
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Kullanım: " << argv[0] << " <hedef_IP_adresi>" << std::endl;
        return 1;
    }

    // Hedef IP adresi komut satırından alınır
    const char* targetIP = argv[1];

    // Bir soket oluştur
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd == -1) {
        perror("Soket oluşturma hatası");
        exit(EXIT_FAILURE);
    }

    // ICMP isteği için bir paket oluştur
    char packet[64];
    struct ip* ipHeader = (struct ip*)packet;
    struct icmphdr* icmpHeader = (struct icmphdr*)(packet + sizeof(struct ip));

    // ICMP paketi için başlık bilgilerini ayarla
    icmpHeader->type = ICMP_ECHO;
    icmpHeader->code = 0;
    icmpHeader->checksum = 0;
    icmpHeader->un.echo.id = getpid();
    icmpHeader->un.echo.sequence = 0;

    // Hedef IP adresini ayarla
    if (inet_pton(AF_INET, targetIP, &(ipHeader->ip_dst)) <= 0) {
        perror("Geçersiz hedef IP adresi");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // ICMP paketi için checksum hesapla
    icmpHeader->checksum = calculateChecksum((unsigned short*)icmpHeader, sizeof(struct icmphdr));

    // ICMP isteği gönder
    if (sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr*)&(ipHeader->ip_dst), sizeof(struct sockaddr_in)) == -1) {
        perror("ICMP isteği gönderme hatası");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // ICMP yanıtını bekleyin ve ekrana yazdırın
    char responsePacket[64];
    ssize_t bytesRead = recv(sockfd, responsePacket, sizeof(responsePacket), 0);
    if (bytesRead == -1) {
        perror("ICMP yanıtı alma hatası");
    } else {
        std::cout << "ICMP yanıtı alındı." << std::endl;
    }

    // Soketi kapat
    close(sockfd);

    return 0;
}
