#include "Socket.h"
#include <sys/epoll.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

namespace fuyou
{
uint32_t pollEvent2Epoll(short events){
    uint32_t e = 0;
    if( events & POLLIN ) 	e |= EPOLLIN;
	if( events & POLLOUT )  e |= EPOLLOUT;
	if( events & POLLHUP ) 	e |= EPOLLHUP;
	if( events & POLLERR )	e |= EPOLLERR;
	if( events & POLLRDNORM ) e |= EPOLLRDNORM;
	if( events & POLLWRNORM ) e |= EPOLLWRNORM;
	return e;
}

short epollEvent2poll(uint32_t events){
    short e = 0;	
	if( events & EPOLLIN ) 	e |= POLLIN;
	if( events & EPOLLOUT ) e |= POLLOUT;
	if( events & EPOLLHUP ) e |= POLLHUP;
	if( events & EPOLLERR ) e |= POLLERR;
	if( events & EPOLLRDNORM ) e |= POLLRDNORM;
	if( events & EPOLLWRNORM ) e |= POLLWRNORM;
	return e;
}

int pollInner(struct pollfd* fds, nfds_t nfds, int timeout){
    if (timeout == 0)
	{
		return poll(fds, nfds, timeout);
	}
	if (timeout < 0)
	{
		timeout = INT_MAX;
	}

	CoroutineScheduler *sched = getSched();
	Coroutine *co = sched -> currCos_;
	
	size_t i = 0;
	for ( ; i < nfds; ++ i) {
	
		struct epoll_event ev;
		ev.events = pollEvent2Epoll(fds[i].events);
		ev.data.fd = fds[i].fd;
		epoll_ctl(sched->pollerfd_, EPOLL_CTL_ADD, fds[i].fd, &ev);

		co->events_ = fds[i].events;
		co -> scheduleScheWait(fds[i].fd, fds[i].events, timeout);
	}
	co -> yield(); 

	for (i = 0;i < nfds;i ++) {
		struct epoll_event ev;
		ev.events = pollEvent2Epoll(fds[i].events);
		ev.data.fd = fds[i].fd;
		epoll_ctl(sched->pollerfd_, EPOLL_CTL_DEL, fds[i].fd, &ev);
		scheDescheWait(fds[i].fd);
	}
    return nfds;
}

int fuyouSocket(int domain, int type, int protocol){
    int fd = socket(domain, type, protocol);
	if (fd == -1) {
		printf("Failed to create a new socket\n");
		return -1;
	}
	int ret = fcntl(fd, F_SETFL, O_NONBLOCK);
	if (ret == -1) {
		close(ret);
		return -1;
	}
	int reuse = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));
	return fd;
}

int fuyouAccept(int fd, struct sockaddr* addr, socklen_t* len){
    int sockfd = -1;
	int timeout = 1;
	// Coroutine *co = getSched()->currCos_;
	while (true) {
		struct pollfd fds;
		fds.fd = fd;
		fds.events = POLLIN | POLLERR | POLLHUP;
		pollInner(&fds, 1, timeout);

		sockfd = accept(fd, addr, len);
		if (sockfd < 0) {
			if (errno == EAGAIN) {
				continue;
			} else if (errno == ECONNABORTED) {
				printf("accept : ECONNABORTED\n");
				
			} else if (errno == EMFILE || errno == ENFILE) {
				printf("accept : EMFILE || ENFILE\n");
			}
			return -1;
		} else {
			break;
		}
	}

	int ret = fcntl(sockfd, F_SETFL, O_NONBLOCK);
	if (ret == -1) {
		close(sockfd);
		return -1;
	}
	int reuse = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));
	
	return sockfd;
}

ssize_t fuyouRecv(int fd, void *buf, size_t len, int flags) {
	
	struct pollfd fds;
	fds.fd = fd;
	fds.events = POLLIN | POLLERR | POLLHUP;

	pollInner(&fds, 1, 1);

	int ret = recv(fd, buf, len, flags);
	if (ret < 0) {
		//if (errno == EAGAIN) return ret;
		if (errno == ECONNRESET) return -1;
		//printf("recv error : %d, ret : %d\n", errno, ret);
		
	}
	return ret;
}

int fuyouConnect(int fd, struct sockaddr* name, socklen_t namelen){
    int ret = 0;
    while(true){
        struct pollfd fds;
		fds.fd = fd;
		fds.events = POLLOUT | POLLERR | POLLHUP;
		pollInner(&fds, 1, 1);

		ret = connect(fd, name, namelen);
		if (ret == 0) break;

		if (ret == -1 && (errno == EAGAIN ||
			errno == EWOULDBLOCK || 
			errno == EINPROGRESS)) {
			continue;
		} else {
			break;
		}
    }
    return ret;
}

ssize_t fuyouSend(int fd, const void *buf, size_t len, int flags){
    size_t sent = 0;
	int ret = send(fd, ((char*)buf) + sent, len - sent, flags);
	if (ret == 0) return ret;
	if (ret > 0) sent += ret;
	while (sent < len) {
		struct pollfd fds;
		fds.fd = fd;
		fds.events = POLLOUT | POLLERR | POLLHUP;
		pollInner(&fds, 1, 1);
		ret = send(fd, ((char*)buf)+sent, len-sent, flags);
		//printf("send --> len : %d\n", ret);
		if (ret <= 0) {			
			break;
		}
		sent += ret;
	}
	if (ret <= 0 && sent == 0) return ret;
	return sent;
}

ssize_t fuyouSendto(int fd, const void *buf, size_t len, int flags,
               const struct sockaddr *dest_addr, socklen_t addrlen){
    size_t sent = 0;
	while (sent < len) {
		struct pollfd fds;
		fds.fd = fd;
		fds.events = POLLOUT | POLLERR | POLLHUP;

		pollInner(&fds, 1, 1);
		int ret = sendto(fd, ((char*)buf)+sent, len-sent, flags, dest_addr, addrlen);
		if (ret <= 0) {
			if (errno == EAGAIN) continue;
			else if (errno == ECONNRESET) {
				return ret;
			}
			printf("send errno : %d, ret : %d\n", errno, ret);
			assert(0);
		}
		sent += ret;
	}
	return sent;
}

ssize_t fuyouRecvfrom(int fd, void *buf, size_t len, int flags,
                 struct sockaddr *src_addr, socklen_t *addrlen) {

	struct pollfd fds;
	fds.fd = fd;
	fds.events = POLLIN | POLLERR | POLLHUP;

	pollInner(&fds, 1, 1);

	int ret = recvfrom(fd, buf, len, flags, src_addr, addrlen);
	if (ret < 0) {
		if (errno == EAGAIN) return ret;
		if (errno == ECONNRESET) return 0;
		
		printf("recv error : %d, ret : %d\n", errno, ret);
		assert(0);
	}
	return ret;

}

int fuyouClose(int fd){
    return close(fd);
}

} // namespace fuyou


