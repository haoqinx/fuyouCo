#include "CoScheduler.h"
#include <thread>
#include <sys/time.h>
#include "Coroutine.h"
#include "Epoll.h"

namespace fuyou
{


CoroutineScheduler::CoroutineScheduler(int stackSize){
    stackSize_ = stackSize ? stackSize : CO_MAX_STACKSIZE;
    pollerfd_ = epollerCreate();
    if(pollerfd_ < 0) {
        perror("epoll create error");
        
    }

}


}