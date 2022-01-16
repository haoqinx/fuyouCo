cmake_minimum_required(VERSION 3.10)
set(SRCS
    serverBase.cpp
)
link_directories(/home/hqin/fuyouCo/fuyouCo/build)

add_executable(server_base ${SRCS})
target_link_libraries(server_base fuyou_co)
