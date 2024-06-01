#include "client.hpp"

#define _RELEASE_

int main(int argc, char** argv){

    /* fetch target time */
    if(argc < 2){
        std::cout<<"Usage : ./client target_time\n";
        exit(1);
    }

    std::time_t target_time;
    target_time = std::stoi(argv[1]);

    std::vector<std::string> files;
    #ifdef _RELEASE_
        auto const dir = "./directory_big";
    #else
        auto dir = argv[2];
    #endif

    traverseDirectory(dir, files, target_time);
    
    #ifndef _RELEASE_
        for(auto i : files){
            std::cout<<i<<"\n";
        }
    #endif
    
    /*send candidate file list to server*/
    sentFile(files);

    return 0;
}