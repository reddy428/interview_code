#include "server.hpp"

bool isEnglishWord(const std::string& word) {
    for (char c : word) {
        if (!std::isalpha(c)) {
            return false; // Non-alphabet character found
        }
    }
    return true; // All characters are alphabetic
}

void count(std::istream& in, std::map<std::string, unsigned int>& dictionary){
    std::string s;

    while (in >> s) {
        if(isEnglishWord(s)){
            ++(dictionary)[s];
        }
    }
}

void countJob(std::vector<std::string>& fileNameList, std::map<std::string, unsigned int> &dictionary){
    for(auto fileName : fileNameList){
         std::ifstream file(fileName);
            count(file, dictionary);
         file.close();
    }    
}