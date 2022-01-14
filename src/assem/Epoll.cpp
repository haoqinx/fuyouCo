#include "Epoll.h"
#include <sys/eventfd.h>
#include <sys/epoll.h>

namespace fuyou
{
int epollerCreate(void){
    return epoll_create1(EPOLL_CLOEXEC);
}

int nty_epoller_ev_register_trigger(void) {

}
}