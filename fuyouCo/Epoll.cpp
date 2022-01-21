#include "Epoll.h"
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include "CoScheduler.h"
#include "Coroutine.h"
#include <assert.h>

namespace fuyou
{

int epollerCreate(void){
    return epoll_create1(EPOLL_CLOEXEC);
}

int epollerCtl(void) {
    CoroutineScheduler* sche = getSched();
    if(! sche -> eventfd_) {
        sche -> eventfd_ = eventfd(0, EFD_NONBLOCK);
        assert(sche -> eventfd_ != -1);
    }
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = sche -> eventfd_;
    int ret = epoll_ctl(sche -> pollerfd_, EPOLL_CTL_ADD, sche -> eventfd_, &ev);
    assert(ret != -1);
    return ret;
}

int epollerWait(struct timespec t){
    CoroutineScheduler* sche = getSched();
    return epoll_wait(sche -> pollerfd_, &*(sche -> eventlist_.begin()), CO_MAX_EVENTS, t.tv_sec*1000.0 + t.tv_nsec/1000000.0);
}
}