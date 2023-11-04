#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>

std::map<std::string, std::string> contentMap;

void readContentConfig(const std::string& configFileName) {
    std::ifstream configFile(configFileName.c_str());
    if (!configFile) {
        std::cerr << "Error opening the configuration file." << std::endl;
        exit(1);
    }

    std::string line;
    while (std::getline(configFile, line)) {
        std::cout << line << std::endl;
        size_t separatorPos = line.find("=");
        if (separatorPos != std::string::npos) {
            std::string path = line.substr(0, separatorPos);
            std::string content = line.substr(separatorPos + 1);
            contentMap[path] = content;
        }
    }
}

std::string getHttpResponse(const std::string& requestedPath) {
    std::cout << requestedPath << std::endl;
    std::map<std::string, std::string>::iterator it = contentMap.find(requestedPath);
    if (it != contentMap.end()) {
        std::cout << it->second << std::endl;
        return "HTTP/1.1 200 OK\r\n\r\n" + it->second;
    } else {
        return "HTTP/1.1 404 Not Found\r\n\r\nFile not found.";
    }
}

int main() {
    // Read the content configuration file
    readContentConfig("content_config.txt");

    // ... Setup socket, bind, and listen ...
        int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket." << std::endl;
        return -1;
    }

    // Bind the socket to an address and port
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080); // Listen on port 8080
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding socket." << std::endl;
        close(serverSocket);
        return -1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error listening for connections." << std::endl;
        close(serverSocket);
        return -1;
    }

    std::cout << "Server listening on port 8080..." << std::endl;

    // Accept and handle incoming connections
    sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSocket;

    while (true) {
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            std::cerr << "Error accepting connection." << std::endl;
            continue;
        }

        // Handle HTTP request
        char buffer[1024];
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        std::cout << buffer << std::endl;

        if (bytesRead > 0) {
            // Extract requested URL path from the HTTP request (e.g., from the "GET" line)
            std::string requestedPath;
            std::string httpRequest(buffer, bytesRead);

            // Find the "GET" line in the HTTP request
            size_t getLineStart = httpRequest.find("GET ");
            if (getLineStart != std::string::npos) {
                // Find the end of the "GET" line
                size_t getLineEnd = httpRequest.find("\r\n", getLineStart);
                
                // Extract the requested path from the "GET" line
                if (getLineEnd != std::string::npos) {
                    // Add 4 to skip "GET " and capture the path until the space
                    requestedPath = httpRequest.substr(getLineStart + 4, getLineEnd - getLineStart - 12);
                }
            }

            // Get the HTTP response based on the requested path

            std::string httpResponse = getHttpResponse(requestedPath);
            std::cout << httpResponse << std::endl;

            // Send the HTTP response to the client
            send(clientSocket, httpResponse.c_str(), httpResponse.size(), 0);
        }

        // Close the client socket
        close(clientSocket);
    }

    // ... Rest of the server code ...
    close(serverSocket);

    return 0;
}
