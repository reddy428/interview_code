#include "server.hpp"

bool cmp(const std::string& lhs, const std::string& rhs)
{
    return (std::filesystem::file_size(lhs) > std::filesystem::file_size(rhs));
}

int main(int argc, char** argv){
    unsigned int threadNum = 0;
    if(argc >= 2){
        threadNum = std::stoi(argv[1]);
    }else{
        std::cout<<"need parameter : number of threads"<<"\n";
        exit(1);
    }
    #ifdef _DEBUG_
        std::cout<<"number of Threds : "<<threadNum<<"\n";
    #endif
 
    // initial thread
    std::vector<unsigned int> threadWorkload(threadNum);
    std::vector<std::map<std::string, unsigned int>> dictionaries(threadNum);
    std::vector<std::vector<std::string>> threadAssignedWork(threadNum);
/*
    for(int i = 0; i < threadNum; i++){
        dictionaries.emplace_back(std::map<std::string, unsigned int>());
        threadAssignedWork.emplace_back(std::vector<std::string>());
    }
*/
    //receive fileNameList from client
    auto fileNameList = socket();

    //Larger file is at front
    std::sort(fileNameList.begin(), fileNameList.end(), cmp);

    // file loading and calculate job loading
    for(auto fileName : fileNameList){
        unsigned int min = findMinIndex(threadWorkload);
        threadWorkload.at(min) += std::filesystem::file_size(fileName);
        (threadAssignedWork.at(min)).push_back(fileName);
    }

    /* assigned job to thread and execute+wait*/
    std::vector<std::thread> threads;

    for(int i = 0; i < threadNum; i++){
         threads.emplace_back(countJob, std::ref(threadAssignedWork[i]), std::ref(dictionaries[i]));
    }

    /*wait thread to join main thread*/
    for (auto& thread : threads) {
        thread.join();
    }

    /* merge output and print*/
    for(int i = 1; i < threadNum; i++){
       mergeMaps(dictionaries[0], dictionaries[i]);
    }

    for(const auto &[x, y] : dictionaries[0]){
            std::cout<<x<<" : "<<y<<"\n";
    }


    return 0;
}   
