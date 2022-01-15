#pragma once
#include <sys/time.h>

namespace fuyou
{
class CoroutineScheduler;
int epollerCreate(void);
int epollerCtl(void);
int epollerWait(struct timespec t);

}