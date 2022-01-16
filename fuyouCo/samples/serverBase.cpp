#include "../Coroutine.h"
#include "../CoScheduler.h"
#include <iostream>
#include <arpa/inet.h>

constexpr int MAX_CLIENT_NUM = 1000000;
int timeSubms(struct timeval tv1, struct timeval tv2){
    return ((tv1.tv_sec - tv2.tv_sec) * 1000 + (tv1.tv_usec - tv2.tv_usec) / 1000);
}

void serverRead(void* arg){
    int fd = *(int*)arg;
    int ret = 0;
    struct pollfd fds;

}
int main(){
    fuyou::Coroutine* co;
    std::cout << "sadasd";
    return 0;
}