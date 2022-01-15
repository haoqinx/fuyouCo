#pragma once 
#include "Share.h"
#include <sys/epoll.h>
#include <vector>
#include <pthread.h>
#include <queue>
#include <set>
#include <list>
#include <memory>
#include "Coroutine.h"

namespace fuyou
{
#define BIT(x)      (1 << (x))
#define CLEARBIT(x) ~(1 << (x))
class Coroutine;
// using SP_Coroutine = std::shared_ptr<Coroutine>;

class CoroutineScheduler{
public:
    CoroutineScheduler(int stackSize, 
                        int pagesize, 
                        uint64_t tiemout,
                        int pollerfd);
    ~CoroutineScheduler();
    Coroutine* scheduleExpired();
    int isDone();
    uint64_t minTimeout();
    int doEpoll();
public:
    struct sleepcmp{
        bool operator()(const Coroutine* co1, const Coroutine* co2){
            return co1 -> sleepUsecs_ > co2 -> sleepUsecs_;
        }
    };
    struct waitcmp{
        bool operator()(const Coroutine* co1, const Coroutine* co2){
            return co1 -> fd_ >co2 -> fd_;
        }
    };
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
    std::queue<Coroutine*> deferCos_;
    std::queue<Coroutine*> readyCos_;
    // co link
    std::list<Coroutine*> busyCos_;
    //
    std::set<Coroutine*, sleepcmp> sleepingCos_;
    std::set<Coroutine*, waitcmp> waitingCos_;

};

class CoroutineComputeSche{

public:
    CoroutineCtx ctx_;
    std::queue<Coroutine*> coroutines_;
    // SP_Coroutine curCoroutine_;
    Coroutine* curCoroutine_;

    pthread_mutex_t runMutex_;
    pthread_cond_t runCond_;

    pthread_mutex_t coMutex_;
    std::list<Coroutine*> computeCos_;

    CoroutineComputeStatus computeStatus_;
};
// shce life cycle
int scheCreate(int stacksize);
void scheSleepDown(Coroutine* co, uint64_t msecs);


};