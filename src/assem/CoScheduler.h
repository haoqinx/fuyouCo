#pragma once 
#include "Share.h"
#include <sys/epoll.h>
#include <vector>
#include <pthread.h>
#include <queue>
#include <set>
#include <list>
#include <memory>

namespace fuyou
{
#define BIT(x) (1 << (x))
class Coroutine;
using SP_Coroutine = std::shared_ptr<Coroutine>;

class CoroutineScheduler{
public:
    CoroutineScheduler(int stackSize);
    ~CoroutineScheduler();

public:
    uint64_t birth_;
    CoroutineCtx ctx_;
    void* stack_;
    size_t stackSize_;
    unsigned int spawnedCors_;
    uint64_t timeout_;
    Coroutine* currCos_;
    int pageSize_;
    int pollerfd_;
    int eventfd_;
    std::vector<struct epoll_event> eventlist_;
    int nevents_;
    int nNewevents_;
    pthread_mutex_t deferMutex_;
    // co queue
    std::queue<SP_Coroutine> readyCos_;
    std::queue<SP_Coroutine> deferCos_;
    // co link
    std::list<SP_Coroutine> busyCos_;
    //
    std::set<SP_Coroutine> sleepingCos_;
    std::set<SP_Coroutine> waitingCos_;

};

class CoroutineComputeSche{

public:
    CoroutineCtx ctx_;
    std::queue<Coroutine> coroutines_;
    SP_Coroutine curCoroutine_;

    pthread_mutex_t runMutex_;
    pthread_cond_t runCond_;

    pthread_mutex_t coMutex_;
    std::list<SP_Coroutine> computeCos_;

    CoroutineComputeStatus computeStatus_;
};

static inline CoroutineScheduler* getSched();

};