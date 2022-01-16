#include "Coroutine.h"
#include <string.h>
#include <assert.h>

namespace fuyou
{
// std::shared_ptr<CoScheduler> gScheduler = std::shared_ptr<CoScheduler>(new CoScheduler());

static void mainfunc(uint32_t low32, uint32_t h32){
    //makecontext函数传递的参数是int
    uintptr_t ptr = (uintptr_t)low32 | ((uintptr_t)h32 << 32);
    CoScheduler* sche = (CoScheduler*)ptr;
    int id = sche -> runningCo_;
    Coroutine* co = sche -> coroutines_[id];
    co -> dofunc();
    sche -> deleteCoroutine(co -> id_);
    -- sche -> numCoroutines_;
    sche -> runningCo_ = -1;
}

CoScheduler::CoScheduler()
            :numCoroutines_(0),
            capacity_(DEFAULT_COROUTINE),
            runningCo_(-1){
    coroutines_.reserve(capacity_);
}

CoScheduler::~CoScheduler(){
    for(auto& co : coroutines_){
        if(co){
            delete(co);
        }
    }
}

int CoScheduler::createCoroutine(coroutineFunc func, void* args){
    Coroutine* newCo = new Coroutine(func, args, this);
    int id;
    if(numCoroutines_ >= capacity_){
        id = capacity_;
        coroutines_.emplace_back(newCo); 
        capacity_ = coroutines_.capacity();
        ++ numCoroutines_;
    }
    else{
        for(int i = 0; i < capacity_; ++ i){
            id = (i + numCoroutines_) % capacity_;
            if(coroutines_[id] == nullptr){
                coroutines_[id] = newCo;
                ++ numCoroutines_;
            }
        }
    }
    newCo -> setId(id);
    return id;
}

void CoScheduler::resume(int id){
    assert(runningCo_ == -1);
    assert(id > 0 && id < capacity_);
    Coroutine* co = coroutines_[id];
    if(! co){
        return;
    }
    int status = co -> getStatus();
    switch(status){
        case COURTINUE_READY:
        {
            getcontext(&co -> ctx_);
            co -> ctx_.uc_stack.ss_sp = stack_;
            co -> ctx_.uc_stack.ss_size = STACK_SIZE;
            co -> ctx_.uc_link = &mainCtx_;
            runningCo_ = id;
            co -> status_ = COURTINUE_RUNNING;
            //设置执行ctx函数
            uintptr_t ptr = (uintptr_t)this;
            makecontext(&co -> ctx_, (void(*)(void)) mainfunc, 2, (uintptr_t)ptr, (uintptr_t)(ptr >> 32));
            swapcontext(&mainCtx_, &co -> ctx_);
            break;
        }
        case COURTINUE_SUSPEND:
        {
            memcpy(stack_ + STACK_SIZE - co -> curSize_, co -> stack_, co -> curSize_);
            runningCo_ = id;
            co -> status_ = COURTINUE_RUNNING;
            swapcontext(&mainCtx_, &co -> ctx_);
            break;
        }
        default:
            assert(0);
            exit(-1);
    }
}


void CoScheduler::deleteCoroutine(int id){
    if(coroutines_[id] != nullptr){
        delete(coroutines_[id]);
        coroutines_[id] = nullptr;
    }
}
void CoScheduler::yield(){
    int id = runningCo_;
    assert(id >= 0);
    Coroutine* co = coroutines_[id];
    assert((char*)& co -> sehcduler_ -> stack_);
    co -> saveStack(stack_ + STACK_SIZE);
    co -> status_ = COURTINUE_SUSPEND;
    runningCo_ = -1;
    swapcontext(&co -> ctx_, &mainCtx_);
}
Coroutine::Coroutine(coroutineFunc func, void* args, CoScheduler* sche)
                    :func_(func),
                    args_(args),
                    sehcduler_(sche),
                    capacity_(0),
                    curSize_(0),
                    status_(COURTINUE_READY),
                    stack_(nullptr){

}

Coroutine::~Coroutine(){
    if(stack_ != nullptr){
        free(stack_);
        stack_ = nullptr;
    }
}

void Coroutine::dofunc(){
    func_(sehcduler_, args_);
}

void Coroutine::saveStack(char* top){
    char dummy = 0;
    ssize_t len = top - &dummy;
    assert(len <= STACK_SIZE);
    if(capacity_ < len){
        free(stack_);
        capacity_ = len;
        stack_ = (char*)malloc(capacity_);
    }
    curSize_ = len;
    memcpy(stack_, &dummy, curSize_);
}


} // namespace fuyou
