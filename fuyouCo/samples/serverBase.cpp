#include "../Coroutine.h"
#include "../CoScheduler.h"
#include "../Socket.h"
#include <iostream>
#include <arpa/inet.h>
#include <string.h>
#include <sys/time.h>
#include <sys/socket.h>

#define _GNU_SOURCE
#include <dlfcn.h>  //对应的头文件
#include <stdio.h>
#include <stdlib.h>

typedef void *(*malloc_t)(size_t size);
malloc_t malloc_f = NULL;
typedef void (*free_t)(void* p);
free_t free_f = NULL;

int enable_malloc_hook = 1;
int enable_free_hook = 1;

void * malloc(size_t size){ 
    if(enable_malloc_hook) //对第三方调用导致的递归进行规避
    {
        enable_malloc_hook = 0;
        //打印上层调用的地址
        void *carrer = __builtin_return_address(0);
        printf("exec malloc [%p ]\n", carrer );
        enable_malloc_hook = 1;
    }
    return malloc_f(size);
}


void free(void * p){
    if(enable_free_hook){
        enable_free_hook = 0;
        void *carrer = __builtin_return_address(0);
        printf("exec free [%p]\n", carrer);
        enable_free_hook = 1;
    }
    free_f(p);
}

void init_malloc_free_hook(){
    //只需要执行一次
    if(malloc_f == nullptr){
        malloc_f = (decltype(malloc_f))dlsym(RTLD_NEXT, "malloc"); //除了RTLD_NEXT 还有一个参数RTLD_DEFAULT
    }
    if(free_f == nullptr)
    {
        free_f =  (decltype(free_f))dlsym(RTLD_NEXT, "free");
    }
    return ;
}


constexpr int MAX_CLIENT_NUM = 1000000;
int timeSubms(struct timeval tv1, struct timeval tv2){
    return ((tv1.tv_sec - tv2.tv_sec) * 1000 + (tv1.tv_usec - tv2.tv_usec) / 1000);
}
void serverRead(void* arg){
    int fd = *(int*)arg;
    int ret = 0;
    struct pollfd fds;
    fds.fd = fd;
    fds.events = POLLIN;
    while(true){
        char buf[1024] = {0};
        ret = fuyou::fuyouRecv(fd, buf, 1024, 0);
        std::cout << "New Msg: " << buf << std::endl;
        if(ret > 0){
            if(fd > MAX_CLIENT_NUM){
                printf("read from server:%s\n", ret, buf);
            }
            std::string hellostring = "";
            hellostring += "HTTP/1.1 200 OK\r\n";
            hellostring += "Content-type: text/plain\r\n";
            hellostring += "Content-Length: " + std::to_string(strlen("Success")) + "\r\n";
            hellostring += "\r\nSuccess";
            // ret = fuyou::fuyouSend(fd, buf, strlen(buf), 0);
            ret = fuyou::fuyouSend(fd, hellostring.c_str(), hellostring.size(), 0);
            fuyou::fuyouClose(fd);
            if(ret == -1){
                fuyou::fuyouClose(fd);
                break;
            }
        }
        else if(ret == 0){
            fuyou::fuyouClose(fd);
            break;
        }
    }
}

void server(void* args){
    unsigned short port = *(unsigned short*) args;
    free(args);
    int fd = fuyou::fuyouSocket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0){
        perror("create socket error");
        return;
    }
    struct sockaddr_in local, remote;
    local.sin_family = AF_INET;
    local.sin_port = htons(port);
    local.sin_addr.s_addr = INADDR_ANY;
    bind(fd, (struct sockaddr*)&local, sizeof(local));
    listen(fd, 2048);
    printf("listen port: %d\n", port);
    struct timeval tv_begin;
    gettimeofday(&tv_begin, nullptr);
    while(true){
        socklen_t len = sizeof(struct sockaddr_in);
        int cli_fd = fuyou::fuyouAccept(fd, (struct sockaddr*)&remote, & len);
        if(cli_fd % 1000 == 999){
            struct timeval tv_cur;
            memcpy(&tv_cur, &tv_begin, sizeof(struct timeval));
			gettimeofday(&tv_begin, NULL);
			int time_used = timeSubms(tv_begin, tv_cur);
			printf("client fd : %d, time_used: %d\n", cli_fd, time_used);
        }
        printf("new client comming\n");
		fuyou::Coroutine *readCo;
		fuyou::coCreate(&readCo, serverRead, &cli_fd);
    }
}

int main(){
    init_malloc_free_hook();
    fuyou::Coroutine* co;
    int i = 0;
	unsigned short base_port = 6666;
	// for (i = 0;i < 100; ++ i) {
	// 	unsigned short *port = (unsigned short*)malloc(sizeof(unsigned short));
	// 	*port = base_port + i;
	// 	fuyou::coCreate(&co, server, port); 
    //     printf("create %d\n", i);
	// }
    unsigned short *port = (unsigned short*)malloc(sizeof(unsigned short));
    *port = base_port;
    fuyou::coCreate(&co, server, port); 
	fuyou::scheRun(); //run
    return 0;
}