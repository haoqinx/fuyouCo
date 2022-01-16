#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <memory>
#include <vector>
#include <functional>
#include <assert.h>

#if defined(__APPLE__) && (defined(__MACH__))
    #include <sys/ucontext.h>
#else
    #include <ucontext.h>
#endif

namespace fuyou
{
const int STACK_SIZE = 1024 * 1024;
const int DEFAULT_COROUTINE = 16;
enum CoStatus {
    COURTINUE_DEAD = 0,
    COURTINUE_READY,
    COURTINUE_RUNNING,
    COURTINUE_SUSPEND
};


class CoScheduler;
using coroutineFunc = std::function<void(CoScheduler*, void*)>;
 
class Coroutine{
public:
    Coroutine(coroutineFunc func, void* args, CoScheduler* sche);
    ~Coroutine();
    
    void saveStack(char* top);
    void dofunc();
    int getStatus(){
        return status_;
    }
    void setId(int id){
        id_ = id;
    }
    
public:
    coroutineFunc func_;
    void* args_;
    CoScheduler* sehcduler_;
    ptrdiff_t capacity_;
    ptrdiff_t curSize_;
    ucontext_t ctx_;
    CoStatus status_;
    char* stack_;
    int id_;
};

class CoScheduler{
public:
    CoScheduler();
    ~CoScheduler();
    int createCoroutine(coroutineFunc func, void* args);
    void deleteCoroutine(int id);
    void resume(int id);
    void yield();
    int currentCo(){
        return runningCo_;
    }
    int getStatus(int id){
        assert(id>=0 && id < capacity_);
        if (coroutines_[id] == nullptr) {
            return COURTINUE_DEAD;
        }
        return coroutines_[id] -> status_;
    }
public:
    CoScheduler(const CoScheduler&) = delete;
    CoScheduler& operator=(const CoScheduler&) = delete;
    char stack_[STACK_SIZE];
    ucontext_t mainCtx_;
    int numCoroutines_;
    int capacity_;
    int runningCo_;
    std::vector<Coroutine*> coroutines_;
};



} // namespace fuyou
