#include "client.hpp"

#define SERVER_ADDR "127.0.0.1"

//getModTime will ruturn mod time if success, else return 0 
std::time_t getModTime(const char * fileName){
    struct stat fileInfo;
    if (stat(fileName, &fileInfo) != 0) {
        std::cerr << "Error: Unable to get file information. : " << fileName <<std::endl;
        return 0;
    }

    // Extract the modification time from the stat structure
    return fileInfo.st_mtime;
}

void traverseDirectory(const std::string& directory, std::vector<std::string>& files, std::time_t targetTime) {  
    
    //getModTime will return 0 if error 
    if(getModTime(directory.c_str()) < targetTime){
        return;
    }

    DIR* dir = opendir(directory.c_str());
    if (dir == nullptr) {
        std::cerr << "Error: Unable to open Directory " << directory << std::endl;
        return;
    }

    dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string fileName = entry->d_name;

        // Skip "." and ".." directories
        if (fileName == ".")continue;
        if (fileName == "..")continue;

        // If the entry is a Directory, recursively traverse it
        std::string fullPath = directory + "/" + fileName;
        if (entry->d_type == DT_DIR) {
            traverseDirectory(fullPath, files, targetTime);
            continue;
        }
        if(getModTime(fullPath.c_str()) < targetTime)continue;

        if(!std::filesystem::is_regular_file(fullPath))continue;

        if(isTextFile(fullPath)){
            files.push_back(fullPath);
        }
    }
    closedir(dir);
}

int sentFile(std::vector<std::string>& msg) {
    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error: Failed to create socket." << std::endl;
        return 1;
    }

    // Connect to the server
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    if (inet_pton(AF_INET, SERVER_ADDR, &serverAddress.sin_addr) <= 0) {
        std::cerr << "Error: Invalid address." << std::endl;
        close(clientSocket);
        return 1;
    }

    if (connect(clientSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error: Connection failed." << std::endl;
        close(clientSocket);
        return 1;
    }

    std::cout << "Connected to server : " << SERVER_ADDR <<std::endl;

    // Serialize vector to JSON
    nlohmann::json j = msg;
    std::string jsonData = j.dump();

    //Convert to network byte order
    size_t fileSize = htonl(jsonData.size());
    
    // Send the file size to the server because it is too big that may be lost
    if (send(clientSocket, &fileSize, sizeof(size_t), 0) == -1) {
        std::cerr << "Error: Send failed." << std::endl;
        close(clientSocket);
        return 1;
    }

    // Send JSON data to server
    if (send(clientSocket, jsonData.c_str(), jsonData.size(), 0) == -1) {
        std::cerr << "Error: Send failed." << std::endl;
        close(clientSocket);
        return 1;
    }

    std::cout << "Message sent to server." << std::endl;

    // Close socket
    close(clientSocket);

    return 0;
}