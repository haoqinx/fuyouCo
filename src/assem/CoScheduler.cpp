#include "CoScheduler.h"
#include <thread>
#include <sys/time.h>
#include "Coroutine.h"
#include <assert.h>

#include <string.h>

namespace fuyou
{

CoroutineScheduler::CoroutineScheduler(int stackSize, int pagesize, 
                        uint64_t tiemout, int pollerfd)
                    :stackSize_(stackSize),
                    pageSize_(pagesize),
                    pollerfd_(pollerfd),
                    spawnedCors_(0),
                    timeout_(tiemout),
                    birth_(coroutineUsecNow()){
    bzero(&ctx_, sizeof(ctx_));

}



}