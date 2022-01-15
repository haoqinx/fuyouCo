#include "CoScheduler.h"
#include <thread>
#include <sys/time.h>
#include "Coroutine.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

namespace fuyou
{

CoroutineScheduler::CoroutineScheduler(int stackSize, int pagesize, 
                        uint64_t tiemout, int pollerfd)
                    :stackSize_(stackSize),
                    pageSize_(pagesize),
                    pollerfd_(pollerfd),
                    spawnedCors_(0),
                    timeout_(tiemout),
                    birth_(coroutineUsecNow()){
    bzero(&ctx_, sizeof(ctx_));

}

CoroutineScheduler::~CoroutineScheduler(){
    if(pollerfd_ > 0){
        close(pollerfd_);
    }
    if(eventfd_ > 0){
        close(eventfd_);
    }

}
 Coroutine* CoroutineScheduler::scheduleExpired(){
    uint64_t diffUsecs = coroutineDiff(birth_, coroutineUsecNow());
    if(! sleepingCos_.empty()){
        Coroutine* co = *sleepingCos_.begin();
        if(co -> sleepUsecs_ <= diffUsecs){
            sleepingCos_.erase(sleepingCos_.begin());
            return co;
        }
        else{
            return nullptr;
        }
    }
    else{
        return nullptr;
    }
    
 }

}