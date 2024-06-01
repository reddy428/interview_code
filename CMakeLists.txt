
cmake_minimum_required(VERSION 3.13)
project(test_cmake CXX)
set(CMAKE_CXX_FLAGS "-std=c++17")


add_executable(server
    src/server/src/count.cpp
    src/server/src/server.cpp
    src/server/src/utility.cpp
)
target_include_directories(server
    PRIVATE src/server/include/
)
add_executable(client
    src/client/src/client.cpp
    src/client/src/utility.cpp
    src/client/src/checktext.cpp
)
target_include_directories(client
    PRIVATE src/client/include/
)