#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <memory>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <map>

//#define _DEBUG_

void count(std::istream& in, std::map<std::string, unsigned int>& dictionary);

void countJob(std::vector<std::string>& fileNameList, std::map<std::string, unsigned int>& dictionary);

unsigned int findMinIndex(const std::vector<unsigned int> intput);

void mergeMaps(std::map<std::string, unsigned int>& map1, const std::map<std::string, unsigned int>& map2);

std::vector<std::string> socket();