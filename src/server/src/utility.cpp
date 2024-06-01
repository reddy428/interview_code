#include "server.hpp"

#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <nlohmann/json.hpp>

unsigned int findMinIndex(const std::vector<unsigned int> input){
    auto it = std::min_element(std::begin(input), std::end(input));
    unsigned int out = std::distance(std::begin(input), it);
    return out;
}

void mergeMaps(std::map<std::string, unsigned int>& map1, const std::map<std::string, unsigned int>& map2) {
    for (const auto& pair : map2) {
        map1[pair.first] += pair.second; // Add the value to map1
    }
}


std::vector<std::string> socket() {
    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error: Failed to create socket." << std::endl;
        exit(1);
    }

    // Bind the socket to an address and port
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);
    if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error: Bind failed." << std::endl;
        close(serverSocket);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error: Listen failed." << std::endl;
        close(serverSocket);
        exit(1);
    }

    std::cout << "Server is listening on port 8080..." << std::endl;

    // Accept incoming connection
    struct sockaddr_in clientAddress;
    socklen_t clientAddressSize = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientAddressSize);
    if (clientSocket == -1) {
        std::cerr << "Error: Accept failed." << std::endl;
        close(serverSocket);
        exit(1);
    }

    std::cout << "Client connected." << std::endl;

    // Receive The number of filesize planning to receive, to prevent socket loss data and so on
    size_t fileSize;
    
    ssize_t bytesReceived = recv(clientSocket, &fileSize, sizeof(size_t), 0);
    if (bytesReceived == -1) {
        std::cerr << "Error: Receive failed." << std::endl;
        close(serverSocket);
        close(clientSocket);
        exit(1);
    }

    //Convert to local byte order
    fileSize = ntohl(fileSize);
    #ifdef _dEBUG_
        std::cout << "filesize : "<<fileSize << std::endl;
    #endif

    // Receive JSON data from client
    char buffer[2048];
    std::string bufferSum;
    size_t hasReceive = 0;

    while(hasReceive < fileSize){
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived == -1) {
            std::cerr << "Error: Receive failed." << std::endl;
            close(serverSocket);
            close(clientSocket);
            exit(1);
        }
        //clean dirty tail
        buffer[bytesReceived] = 0;
        hasReceive += bytesReceived;
        bufferSum.append(buffer);
    }
    // Close sockets
    close(clientSocket);
    close(serverSocket);
    
    //check if the receive data matching the fileSize
    if(bufferSum.size() != fileSize){
        std::cout<<"file is not match : "<<bufferSum<<" "<<fileSize<<std::endl;
        exit(1);
    }

    //std::cout << "Received from client: " << bufferSum.size() << std::endl;

    // Parse JSON data
    nlohmann::json j = nlohmann::json::parse(bufferSum);

    // Deserialize JSON to vector of strings
    std::vector<std::string> message = j.get<std::vector<std::string>>();

    // Print received strings
    #ifdef _DEBUG_
        for (const auto &str : message) {
            std::cout << str << std::endl;
        }
    #endif

    return message;
}