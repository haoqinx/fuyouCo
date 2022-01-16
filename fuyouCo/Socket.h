#pragma once 
#include "Coroutine.h"
#include "CoScheduler.h"
namespace fuyou
{
uint32_t pollEvent2Epoll(short events);
short epollEvent2poll(uint32_t events);
int pollInner(struct pollfd* fds, nfds_t nfds, int timeout);
int fuyouSocket(int domain, int type, int protocol);
}