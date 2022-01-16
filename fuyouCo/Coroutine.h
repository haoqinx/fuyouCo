#pragma once
#include "Share.h"
#include <functional>
#include <memory>
#include <sys/poll.h>
#include <vector>
namespace fuyou
{
class CoroutineScheduler;
class CoroutineComputeSche;

constexpr unsigned int CO_MAX_EVENTS = 1024 * 1024;
constexpr unsigned int CO_MAX_STACKSIZE = 16 * 1024;

using proc_coroutine = std::function<void(void*)>;

class Coroutine{
public: 
    Coroutine(CoroutineScheduler* sche, size_t stackSize, int id, 
                proc_coroutine func, int fd, unsigned short events, void* args);
    ~Coroutine();
    void dofunc(){
        func_(args_);
    }
    void yield();
    void setStatus();
    int resume();
    void init();
    void renice();
    // void sleep(uint64_t msecs);
    void scheduleScheSleepdown(uint64_t msecs);
    void scheduleDeschedAndSleepdown();
    void scheduleScheWait(int fd, unsigned short events, uint64_t timeout);

public:
    // using SP_CoroutineComputeSche = std::shared_ptr<CoroutineComputeSche>;
    // using SP_pollfd = std::shared_ptr<struct pollfd>;
    // using SP_CoroutineScheduler = std::shared_ptr<CoroutineScheduler>;
public:
    struct IOinfo{
        void* buf;
        size_t nbytes_;
        int fd_;
        int ret_;
        int err_;
    };
public:
    CoroutineCtx ctx_;
    proc_coroutine func_;
    void* args_;
    void* data_;
    size_t stackSize_;
    size_t lastStackSize_;
    //status
    CoroutineStatus status_;
    // std::weak_ptr<CoroutineScheduler> sche_;
    // scheduler
    CoroutineScheduler* sche_;
    // attr
    uint64_t birth_;
    uint64_t id_;
    //poll
    int fd_;
    unsigned short events_;

    char funcname_[64];
    Coroutine* joinCo_;
    void** coExitPtr_;
    void* stack_;
    void* ebp_;
    uint32_t ops_;
    uint64_t sleepUsecs_;

    //io
    IOinfo io_;
    // SP_CoroutineComputeSche compSche_;
    CoroutineComputeSche* compSche_;
    int fdsReady_;
    std::vector<struct pollfd> pfds_; 
    nfds_t nfds_;

};
// coroutine life cycle
int coCreate(Coroutine** newCo, proc_coroutine func, void* args);
// inline int coSleepcmp(Coroutine* co1, Coroutine* co2);
// inline int coWaitcmp(Coroutine* co1, Coroutine* co2);
//sche life cycle
int scheCreate(int stacksize);
void scheFree(CoroutineScheduler*);
void scheRun();
// void sleep(uint64_t msecs);
// helper
inline uint64_t coroutineDiff(uint64_t t1, uint64_t t2);
inline uint64_t coroutineUsecNow();
inline void coroutineMadvise(Coroutine* co);
void schedKeyDestructor(void *data);
void schedKeyCreator(void);
void scheSleepDown(Coroutine* co, uint64_t msecs);
inline CoroutineScheduler* getSched();
// find co by fd
Coroutine* scheSearchWait(int fd);
// find co in waitset and sleep
Coroutine* scheDescheWait(int fd);

} // namespace fuyou

