// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <string>
// #include <cstdlib>

// struct ServerConfig {
//     int port;
//     std::string document_root;
// };

// ServerConfig readConfig(const std::string& configFileName) {
//     ServerConfig config;
//     std::ifstream configFile(configFileName.c_str());

//     if (!configFile) {
//         std::cerr << "Error opening the configuration file." << std::endl;
//         exit(1);
//     }

//     std::string line;
//     while (std::getline(configFile, line)) {
//         std::istringstream iss(line);
//         std::string key, value;
//         if (iss >> key) {
//             if (key == "port:") {
//                 if (iss >> config.port) {
//                     // Successfully read the port value
//                 } else {
//                     std::cerr << "Error reading port value." << std::endl;
//                 }
//             } else if (key == "document_root:") {
//                 if (iss >> config.document_root) {
//                     // Successfully read the document_root value
//                 } else {
//                     std::cerr << "Error reading document_root value." << std::endl;
//                 }
//             }
//         }
//     }

//     return config;
// }

// int main() {
//     ServerConfig config = readConfig("test.txt");
    
//     std::cout << "Port: " << config.port << std::endl;
//     std::cout << "Document Root: " << config.document_root << std::endl;
    
//     // ... Rest of your server code ...
    
//     return 0;
// }


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct ServerConfig {
    int port;
    std::string document_root;
};

ServerConfig readConfig(const std::string& configFileName) {
    ServerConfig config;
    std::ifstream configFile(configFileName.c_str());

    if (!configFile) {
        std::cerr << "Error opening the configuration file." << std::endl;
        exit(1);
    }

    std::string line;
    while (std::getline(configFile, line)) {
        std::istringstream iss(line);
        std::string key, value;
        if (iss >> key) {
            if (key == "port:") {
                if (iss >> config.port) {
                    // Successfully read the port value
                } else {
                    std::cerr << "Error reading port value." << std::endl;
                }
            } else if (key == "document_root:") {
                if (iss >> config.document_root) {
                    // Successfully read the document_root value
                } else {
                    std::cerr << "Error reading document_root value." << std::endl;
                }
            }
        }
    }

    return config;
}

std::string getHttpResponse(const std::string& filePath) {
    std::ifstream file(filePath.c_str());
    if (!file) {
        return "HTTP/1.1 404 Not Found\r\n\r\nFile not found.";
    }

    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Length: " << file.tellg() << "\r\n\r\n";
    file.seekg(0, std::ios::beg);
    response << file.rdbuf();

    return response.str();
}

int main() {
    // Read the configuration file
    ServerConfig config = readConfig("test.txt");
    
    std::cout << "Port: " << config.port << std::endl;
    std::cout << "Document Root: " << config.document_root << std::endl;

    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket." << std::endl;
        return 1;
    }

    // Bind the socket to an address and port
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(config.port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding socket." << std::endl;
        close(serverSocket);
        return 1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error listening for connections." << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Server listening on port " << config.port << "..." << std::endl;

    while (true) {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == -1) {
            std::cerr << "Error accepting connection." << std::endl;
            continue;
        }

        std::cout << "New client connected." << std::endl;

        // Handle HTTP request
        char buffer[1024];
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesRead > 0) {
            // Extract requested URL path from the HTTP request (e.g., from the "GET" line)
            // Construct the file path within the document root based on the URL path
            std::string httpRequest(buffer, bytesRead);

            // Find the "GET" line in the HTTP request
            std::string requestedPath;
            size_t getLineStart = httpRequest.find("GET ");
            if (getLineStart != std::string::npos) {
                // Find the end of the "GET" line
                size_t getLineEnd = httpRequest.find("\r\n", getLineStart);
                
                // Extract the requested path from the "GET" line
                if (getLineEnd != std::string::npos) {
                    // Add 4 to skip "GET " and capture the path until the space
                    requestedPath = httpRequest.substr(getLineStart + 4, getLineEnd - getLineStart - 4);
                }
            }
            std::string filePath = config.document_root + requestedPath;

            // Get the HTTP response based on the file content
            std::string httpResponse = getHttpResponse(filePath);

            // Send the HTTP response to the client
            send(clientSocket, httpResponse.c_str(), httpResponse.size(), 0);
        }

        // Close the client socket
        close(clientSocket);
    }

    // Close the server socket (never reached in this example)
    close(serverSocket);

    return 0;
}
