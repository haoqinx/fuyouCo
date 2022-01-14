#include "Coroutine.h"
namespace fuyou
{
//
static void _exec(void *lt){
// lvm mode
#if defined(__lvm__) && defined(__x86_64__)
    __asm__("movq 16(%%rbp), %[lt]" : [lt] "=r" (lt));
#endif
    Coroutine* co = (Coroutine*)lt;
    co -> dofunc();
    co -> status_ = (CoroutineStatus)(int(co -> status_) | 
                    (BIT(COROUTINE_STATUS_EXITED) | 
                    BIT(COROUTINE_STATUS_FDEOF) | 
                    BIT(COROUTINE_STATUS_DETACH)));
    
    co -> yield();
}

Coroutine::~Coroutine(){
    if(sche_.lock()){
        sche_.lock() -> spawnedCors_ --;
    }
    else{
        perror("no shce");
    }
    if(stack_ != nullptr){
        free(stack_);
        stack_ = nullptr;
    }

}

void Coroutine::yield(){

}
}