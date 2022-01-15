#include "Coroutine.h"
#include <sys/time.h>
#include <pthread.h>
#include <sys/mman.h>
#include <assert.h>
#include <unistd.h>
#include "Epoll.h"
#include "CoScheduler.h"

namespace fuyou
{

pthread_key_t global_sched_key;
static pthread_once_t sched_key_once = PTHREAD_ONCE_INIT;

//static functions
static inline uint64_t coroutineDiff(uint64_t t1, uint64_t t2){
    return t2 - t1;
}

inline uint64_t coroutineUsecNow(){
    struct timeval t1;
    gettimeofday(&t1, nullptr);
    return t1.tv_sec * 1000000 + t1.tv_usec;
}

static inline void coroutineMadvise(Coroutine* co){
    size_t current_stack = (char*)(co -> stack_ + co -> stackSize_) - (char*)(co -> ctx_.esp);
    assert(current_stack <= co -> stackSize_);
    if(current_stack < co -> lastStackSize_ &&
        co -> lastStackSize_ > co -> sche_ -> pageSize_){
        size_t tmp = current_stack + (- current_stack & (co -> sche_ -> pageSize_ - 1));
        assert(madvise(co -> stack_, co -> stackSize_ - tmp, MADV_DONTNEED) == 0);        
    }
    co -> lastStackSize_ = current_stack;
}

static void schedKeyDestructor(void *data){
    free(data);
}

static void schedKeyCreator(void){
    assert(pthread_key_create(&global_sched_key, schedKeyDestructor) == 0);
	assert(pthread_setspecific(global_sched_key, NULL) == 0);
	return ;
}

// void scheSleepDown(Coroutine* co, uint64_t msecs){
//     uint64_t usecs = msecs * 1000u;
//     auto sleeps = co -> sche_ -> sleepingCos_;
//     auto co_find = sleeps.find(co);
//     if(co_find != sleeps.end()){
//         sleeps.erase(co);
//     }
//     co -> sleepUsecs_ = coroutineDiff(co -> sche_ -> birth_, coroutineUsecNow()) + usecs;
//     do{
//         auto res = sleeps.insert(co);
//         if(res.second){
//             printf("sleep_usecs %u\n", co -> sleepUsecs_);
//         }
//         co -> status_ = (CoroutineStatus)((unsigned int)co -> status_ | 
//                     BIT(COROUTINE_STATUS_SLEEPING));
//     }while(false);
// }

extern "C"{
    int _switch(CoroutineCtx* new_ctx, CoroutineCtx *cur_ctx);
}

static void _exec(void *lt);

//_switch函数定义
#ifdef __i386__
__asm__ (
"    .text                                  \n"
"    .p2align 2,,3                          \n"
".globl _switch                             \n"
"_switch:                                   \n"
"__switch:                                  \n"
"movl 8(%esp), %edx      # fs->%edx         \n"
"movl %esp, 0(%edx)      # save esp         \n"
"movl %ebp, 4(%edx)      # save ebp         \n"
"movl (%esp), %eax       # save eip         \n"
"movl %eax, 8(%edx)                         \n"
"movl %ebx, 12(%edx)     # save ebx,esi,edi \n"
"movl %esi, 16(%edx)                        \n"
"movl %edi, 20(%edx)                        \n"
"movl 4(%esp), %edx      # ts->%edx         \n"
"movl 20(%edx), %edi     # restore ebx,esi,edi      \n"
"movl 16(%edx), %esi                                \n"
"movl 12(%edx), %ebx                                \n"
"movl 0(%edx), %esp      # restore esp              \n"
"movl 4(%edx), %ebp      # restore ebp              \n"
"movl 8(%edx), %eax      # restore eip              \n"
"movl %eax, (%esp)                                  \n"
"ret                                                \n"
);
#elif defined(__x86_64__)
__asm__ (
"    .text                                  \n"
"       .p2align 4,,15                                   \n"
".globl _switch                                          \n"
".globl __switch                                         \n"
"_switch:                                                \n"
"__switch:                                               \n"
"       movq %rsp, 0(%rsi)      # save stack_pointer     \n"
"       movq %rbp, 8(%rsi)      # save frame_pointer     \n"
"       movq (%rsp), %rax       # save insn_pointer      \n"
"       movq %rax, 16(%rsi)                              \n"
"       movq %rbx, 24(%rsi)     # save rbx,r12-r15       \n"
"       movq %r12, 32(%rsi)                              \n"
"       movq %r13, 40(%rsi)                              \n"
"       movq %r14, 48(%rsi)                              \n"
"       movq %r15, 56(%rsi)                              \n"
"       movq 56(%rdi), %r15                              \n"
"       movq 48(%rdi), %r14                              \n"
"       movq 40(%rdi), %r13     # restore rbx,r12-r15    \n"
"       movq 32(%rdi), %r12                              \n"
"       movq 24(%rdi), %rbx                              \n"
"       movq 8(%rdi), %rbp      # restore frame_pointer  \n"
"       movq 0(%rdi), %rsp      # restore stack_pointer  \n"
"       movq 16(%rdi), %rax     # restore insn_pointer   \n"
"       movq %rax, (%rsp)                                \n"
"       ret                                              \n"
);
#endif

//
static void _exec(void *lt){
// lvm mode
#if defined(__lvm__) && defined(__x86_64__)
    __asm__("movq 16(%%rbp), %[lt]" : [lt] "=r" (lt));
#endif
    Coroutine* co = (Coroutine*)lt;
    co -> dofunc();
    co -> status_ = (CoroutineStatus)(unsigned int)((co -> status_) | 
                    (BIT(COROUTINE_STATUS_EXITED) | 
                    BIT(COROUTINE_STATUS_FDEOF) | 
                    BIT(COROUTINE_STATUS_DETACH)));
    
    co -> yield();
}

inline CoroutineScheduler* getSched(){
    return (CoroutineScheduler*)pthread_getspecific(global_sched_key);
}

Coroutine::Coroutine(CoroutineScheduler* sche, int stackSize, int id, 
                    proc_coroutine func, int fd, unsigned short events, void* args):
                    sche_(sche),
                    stackSize_(stackSize),
                    status_((CoroutineStatus) BIT(COROUTINE_STATUS_NEW)),
                    id_(id),
                    func_(func),
                    fd_(fd),
                    events_(events),
                    args_(args),
                    birth_(coroutineUsecNow()){

}

Coroutine::~Coroutine(){
    if(sche_){
        sche_ -> spawnedCors_ --;
    }
    else{
        perror("no shce");
    }
    if(stack_ != nullptr){
        free(stack_);
        stack_ = nullptr;
    }

}
void Coroutine::init(){
    void** stack = (void**)(stack_ + stackSize_);
    stack[-3] = nullptr;
    stack[-2] = (void*)this;

    ctx_.esp = (void*) stack - (4 * sizeof(void*));
    ctx_.ebp = (void*) stack - (3 * sizeof(void*));
    ctx_.eip = (void*) _exec;
    status_ = (CoroutineStatus) BIT(COROUTINE_STATUS_READY);
}

void Coroutine::yield(){
    ops_ = 0;
    _switch(&(sche_ -> ctx_), &ctx_);
}

int Coroutine::resume(){
    if((unsigned int)status_ & BIT(COROUTINE_STATUS_NEW)){
        init();
    }
    CoroutineScheduler* sche = getSched();
    sche -> currCos_ = this;
    _switch(&ctx_, &(sche_ -> ctx_));
    sche -> currCos_ = nullptr;

    coroutineMadvise(this);
    if((unsigned int)status_ & BIT(COROUTINE_STATUS_EXITED)){
        if((unsigned int)status_ & BIT(COROUTINE_STATUS_DETACH)){
            printf("resume...");
            //nty_coroutine_free(co);
        }
        return -1;
    }
    return 0;
}

void Coroutine::renice(){
    ++ ops_;
    if(ops_ < 5) return;
    printf("%d renice....", id_);
    sche_ -> readyCos_.push(this);
    yield();
}

void Coroutine::scheduleScheSleepdown(uint64_t msecs){
    uint64_t usecs = msecs * 1000u;
    auto sset = this -> sche_ -> sleepingCos_;
    auto co_tmp = sset.find(this);
    if(co_tmp != sset.end()){
        sset.erase(co_tmp);
    }
    this -> sleepUsecs_ = coroutineDiff(this -> sche_ -> birth_, coroutineUsecNow()) + usecs;
    do{
        auto res = sset.insert(this);
        if(res.second){
            printf("sleep_usecs %u\n", this -> sleepUsecs_);
            this -> sleepUsecs_ ++;
            continue;
        }
        this -> status_ = (CoroutineStatus)((unsigned int)this -> status_ | 
                    BIT(COROUTINE_STATUS_SLEEPING));
    }while(true);
}

void Coroutine::scheduleDeschedAndSleepdown(){
    if((int)status_ & BIT(COROUTINE_STATUS_SLEEPING)){
        this -> sche_ -> sleepingCos_.erase(this);
        status_ = CoroutineStatus((int)status_ & CLEARBIT(COROUTINE_STATUS_SLEEPING));
        status_ = CoroutineStatus((int)status_ | BIT(COROUTINE_STATUS_READY));
        status_ = CoroutineStatus((int)status_ & CLEARBIT(COROUTINE_STATUS_EXPIRED));
    }
}
// handle current coroutine
//sleep
void curCoroutineSleep(uint64_t msecs){
    Coroutine* co = getSched() -> currCos_;
    if(msecs == 0){
        co -> sche_ -> readyCos_.push(co);
        co -> yield();
    }
    else{
        // scheSleepDown(co, msecs);
        co -> scheduleScheSleepdown(msecs);
    }
}
//detach
void curCoroutineDetach(){
    Coroutine* co = getSched() -> currCos_;
    co -> status_ = (CoroutineStatus)((unsigned int)co -> status_ | 
                    BIT(COROUTINE_STATUS_DETACH));
}

int coCreate(Coroutine** newCo, proc_coroutine func, void* args){
    assert(pthread_once(&sched_key_once, schedKeyCreator) == 0);
    CoroutineScheduler* sche = getSched();
    if(sche == nullptr){
        scheCreate(0);
        sche = getSched();
        if (sche == nullptr) {
			printf("Failed to create scheduler\n");
			return -1;
		}
    }
    
    Coroutine* co = new Coroutine(sche, 
                                  sche -> stackSize_, 
                                  sche -> spawnedCors_ ++,
                                  func,
                                  -1, //fd
                                  0,//events
                                  args);
    *newCo = co;
    int ret = posix_memalign(&co -> stack_, getpagesize(), sche -> stackSize_);
    if(ret) {
        printf("failed to allocate stack for new co\n");
        delete(co);
        return -3;
    }
    return 0;
}
// inline int coSleepcmp(Coroutine* co1, Coroutine* co2){
//     if (co1 -> sleepUsecs_ < co2 -> sleepUsecs_) {
// 		return -1;
// 	}
// 	if (co1 -> sleepUsecs_ == co2 -> sleepUsecs_) {
// 		return 0;
// 	}
// 	return 1;
// }
// inline int coWaitcmp(Coroutine* co1, Coroutine* co2){
// 	if (co1->fd_ < co2->fd_) return -1;
// 	else if (co1->fd_ == co2->fd_) return 0;
// 	else return 1;
// }
int scheCreate(int stacksize){
    int ssize = stacksize ? stacksize : CO_MAX_STACKSIZE;
    int pollerfd = epollerCreate();
    if(pollerfd < 0) {
        perror("epoll create error");
        return -2;
    }
    int pagesize = getpagesize();
    int timeout = 3000000u;

    CoroutineScheduler* sched = new CoroutineScheduler(ssize, 
                                                      pagesize, 
                                                      timeout, 
                                                      pollerfd);
    if(sched == nullptr){
        printf("Fail to init scheduler");
        return -1;
    }
    assert(pthread_setspecific(global_sched_key, sched) == 0);
}

Coroutine* scheSearchWait(int fd){
    auto sche = getSched();
    auto wset = sche -> waitingCos_;
    auto it = wset.begin();
    for(; it != wset.end(); ++ it){
        if((*it) -> fd_ == fd) break;
    }
    if(it != wset.end()){
        (*it) -> status_ = (CoroutineStatus)0;
        return *it;
    }
    return nullptr;
}
Coroutine* scheDescheWait(int fd){
    Coroutine* co = scheSearchWait(fd);
    auto wset = getSched() -> waitingCos_;
    if(co != nullptr){
        wset.erase(co);
        co -> scheduleDeschedAndSleepdown();
    }
    else{
        printf("cannot find fd in wait set");
    }
    return co;
}

void Coroutine::scheduleScheWait(int fd, unsigned short events, uint64_t timeout){
    if((int)status_ & BIT(COROUTINE_STATUS_WAIT_READ) || 
        (int)status_ & BIT(COROUTINE_STATUS_WAIT_WRITE)){
        printf("Unexpected event. lt id %lld fd %lld already in %ld state\n",
            id_, fd_, status_);
        assert(0);
    }

    if(events & POLLIN){
        status_ = (CoroutineStatus)((int)status_ | COROUTINE_STATUS_WAIT_READ);
    }
    else if(events | POLLOUT){
        status_ = (CoroutineStatus)((int)status_ | COROUTINE_STATUS_WAIT_WRITE);
    }
    else{
        printf("events: %d\n", events);
    }
    fd_ = fd;
    events_ = events;
    auto co_tmp = sche_ -> waitingCos_.find(this);
    assert(co_tmp == sche_ -> waitingCos_.end());
    if(timeout == 1) return;

}

}