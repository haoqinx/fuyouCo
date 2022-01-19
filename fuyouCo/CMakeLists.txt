cmake_minimum_required(VERSION 3.10)
set(
    LIB_SRC
    Coroutine.cpp
    CoScheduler.cpp
    Epoll.cpp
    Socket.cpp
)
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb3")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
add_definitions(-std=c++11)
add_library(fuyou_co ${LIB_SRC})
target_link_libraries(fuyou_co pthread rt)
set_target_properties(fuyou_co PROPERTIES OUTPUT_NAME "fuyou_co")
