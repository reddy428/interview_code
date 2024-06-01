#include <iostream>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <ctime>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <nlohmann/json.hpp>

std::time_t getModTime(const char * FileName);

void traverseDirectory(const std::string& directory, std::vector<std::string>& files, std::time_t target_time);

int sentFile(std::vector<std::string>& msg);

bool isTextFile(const std::string& FileName);