cmake_minimum_required(VERSION 3.10)
set(SRCS
    Coroutine.cpp
    CoScheduler.cpp
    Epoll.cpp
    Main.cpp
)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb -pthread -std=c++11")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

add_executable(main ${SRCS})