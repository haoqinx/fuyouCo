#include "CoScheduler.h"
#include <thread>
#include <sys/time.h>
#include "Coroutine.h"

namespace fuyou
{

pthread_key_t global_sched_key;
static pthread_once_t sched_key_once = PTHREAD_ONCE_INIT;
static inline CoroutineScheduler* getSched(){
    return (CoroutineScheduler*)pthread_getspecific(global_sched_key);
}

static inline uint64_t coroutineDiff(uint64_t t1, uint64_t t2){
    return t2 - t1;
}

static inline uint64_t coroutineUsecNow(){
    struct timeval t1;
    gettimeofday(&t1, nullptr);
    return t1.tv_sec * 1000000 + t1.tv_usec;
}

CoroutineScheduler::CoroutineScheduler(int stackSize):
                                        stackSize_(stackSize){
    if(stackSize > CO_MAX_STACKSIZE);
}

}