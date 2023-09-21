#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>

int main() {
	// Soket oluşturma
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0) {
		std::cerr << "Error creating socket." << std::endl;
		return 1;
	}

	// Soket adresini ayarlama
	struct sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(8080);

	// Soketi bağlama
	if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
		std::cerr << "Error binding socket." << std::endl;
		close(serverSocket);
		return 1;
	}

	// Soketi dinleme moduna geçirme
	if (listen(serverSocket, 5) < 0) {
		std::cerr << "Error listening on socket." << std::endl;
		close(serverSocket);
		return 1;
	}

	std::cout << "Server is listening on port 8080..." << std::endl;

	// pollfd yapısı oluşturma
	struct pollfd pollFds[1];
	pollFds[0].fd = serverSocket;
	pollFds[0].events = POLLIN;

	//while (true) {
		// poll işlemi (timeout süresi 1 saniye)
		int pollResult = poll(pollFds, 1, -1); // 1000 ms (1 saniye) bekler
		if (pollResult < 0) {
			std::cerr << "Error in poll." << std::endl;
			//break;
		}
		std::cout << "..";
		if (pollResult > 0) {
			if (pollFds[0].revents & POLLIN) {
				// Soketten gelen veriyi oku ve ekrana yazdır
				char buffer[1024];
				int bytesRead = recv(serverSocket, buffer, sizeof(buffer), 0);
				if (bytesRead < 0) {
					std::cerr << "Error reading from socket." << std::endl;
					//break;
				} else if (bytesRead == 0) {
					// Bağlantı kapandı
					std::cout << "Client disconnected." << std::endl;
					//break;
				} else {
					std::cout << "Received data: " << std::string(buffer, bytesRead) << std::endl;
				}
			}
		}
	//}

	// Soketi kapat
	close(serverSocket);

	return 0;
}