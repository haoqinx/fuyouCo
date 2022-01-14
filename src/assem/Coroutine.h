#pragma once
#include "Share.h"
#include "CoScheduler.h"
#include <functional>
#include <memory>
#include <sys/poll.h>
namespace fuyou
{
constexpr unsigned int CO_MAX_EVENTS = 1024 * 1024;
constexpr unsigned int CO_MAX_STACKSIZE = 16 * 1024;



using proc_coroutine = std::function<void(void*)>;

class Coroutine{
public: 
    Coroutine();
    ~Coroutine();
    void dofunc(){
        func_(args_);
    }
    void yield();
    void setStatus();
    int resume();
    void init();
    
public:
    using SP_CoroutineComputeSche = std::shared_ptr<CoroutineComputeSche>;
    using SP_pollfd = std::shared_ptr<struct pollfd>;
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
    std::weak_ptr<CoroutineScheduler> sche_;
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
    SP_CoroutineComputeSche compSche_;
    int fdsReady_;
    SP_pollfd pfds_; 
    nfds_t nfds_;

};

} // namespace fuyou

