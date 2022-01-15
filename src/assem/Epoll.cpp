#include "Epoll.h"
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include "CoScheduler.h"
#include "Coroutine.h"

namespace fuyou
{

int epollerCreate(void){
    return epoll_create1(EPOLL_CLOEXEC);
}

int epollerCtl(void) {
    CoroutinueScheduler* sche = get
}
}