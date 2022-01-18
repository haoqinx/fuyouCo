#pragma once 
#include "Coroutine.h"
#include "CoScheduler.h"
#include <sys/socket.h>
namespace fuyou
{
static uint32_t pollEvent2Epoll(short events);
static short epollEvent2poll(uint32_t events);
static int pollInner(struct pollfd* fds, nfds_t nfds, int timeout);
int fuyouSocket(int domain, int type, int protocol);
int fuyouAccept(int fd, struct sockaddr* addr, socklen_t* len);
int fuyouConnect(int fd, struct sockaddr* name, socklen_t namelen);
ssize_t fuyouSend(int fd, const void *buf, size_t len, int flags);
ssize_t fuyouSendto(int fd, const void *buf, size_t len, int flags,
               const struct sockaddr *dest_addr, socklen_t addrlen);
ssize_t fuyouRecv(int fd, void *buf, size_t len, int flags);
ssize_t fuyouRecvfrom(int fd, void *buf, size_t len, int flags,
                 struct sockaddr *src_addr, socklen_t *addrlen);
int fuyouClose(int fd);
}