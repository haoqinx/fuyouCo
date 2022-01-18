cmake_minimum_required(VERSION 3.10)
set(SRCS
    serverBase.cpp
)
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
link_directories(/home/hqin/fuyouCo/fuyouCo/build)
add_executable(server_base ${SRCS})
target_link_libraries(server_base fuyou_co)
target_link_libraries(server_base pthread rt)