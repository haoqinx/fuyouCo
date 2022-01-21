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


extern void * malloc(size_t size){ 
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


extern void free(void * p){
    if(enable_free_hook){
        enable_free_hook = 0;
        void *carrer = __builtin_return_address(0);
        printf("exec free [%p]\n", carrer);
        enable_free_hook = 1;
    }
    free_f(p);
}

extern void init_malloc_free_hook(){
    //只需要执行一次
    if(malloc_f == NULL){
        malloc_f = dlsym(RTLD_NEXT, "malloc"); //除了RTLD_NEXT 还有一个参数RTLD_DEFAULT
    }
    if(free_f == NULL)
    {
        free_f =  dlsym(RTLD_NEXT, "free");
    }
    return ;
}

