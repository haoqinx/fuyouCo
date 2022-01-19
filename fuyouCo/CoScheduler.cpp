#include "CoScheduler.h"
#include <thread>
#include <sys/time.h>
#include "Coroutine.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "Epoll.h"

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
    busyCos_.clear();
    sleepingCos_.clear();
    waitingCos_.clear();
    eventlist_.reserve(CO_MAX_EVENTS);
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
        Coroutine* co = *(sleepingCos_.begin());
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
int CoroutineScheduler::isDone(){
    return (waitingCos_.empty() && busyCos_.empty()
            && sleepingCos_.empty() && readyCos_.empty());
}
uint64_t CoroutineScheduler::minTimeout(){
    uint64_t diffUsecs = coroutineDiff(birth_, coroutineUsecNow());
    uint64_t min = timeout_;
    if(! sleepingCos_.empty()){
        Coroutine* co = *(sleepingCos_.begin());
        min = co -> sleepUsecs_;
        if(min > diffUsecs){
            return min - diffUsecs;
        }
    }   
    else{
        return min;
    }   
}

int CoroutineScheduler::doEpoll(){
    printf("start do epoll\n");
    nNewevents_ = 0;
    struct timespec t = {0, 0};
    uint64_t usecs = minTimeout();
    printf("is empty?? %d\n", readyCos_.empty());
    if(usecs && readyCos_.empty()){
        t.tv_sec = usecs / 1000000u;
		if (t.tv_sec != 0) {
			t.tv_nsec = (usecs % 1000u) * 1000u;
		} 
        else {
			t.tv_nsec = usecs * 1000u;
		}
    }
    else {
		return 0;
	}
    int nready = 0;
    while(true){
        nready = epollerWait(t);
        if(nready == -1){
            if(errno == EINTR) continue;
            else{
                perror("epoll wait error");
                exit(-1);
            }
        }
        break;
    }
    nevents_ = 0;
    nNewevents_ = nready;
    return 0;
}

}