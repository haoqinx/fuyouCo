#include "Coroutine.h"
#include <stdio.h>
#include <memory>
using namespace fuyou;
using SP_Scheduler = std::shared_ptr<CoScheduler>;
struct args {
	int n;
};

static void foo(SP_Scheduler s, void *ud) {
	struct args * arg = (struct args*)ud;
	int start = arg->n;
	int i;
	for (i=0;i<5;i++) {
		printf("coroutine %d : %d\n",s->currentCo() , start + i);
		// 切出当前协程
		s->yield();
	}
}

static void test(SP_Scheduler s) {
	struct args arg1 = { 0 };
	struct args arg2 = { 100 };

	// 创建两个协程
	int co1 = s -> createCoroutine(foo, &arg1);
	int co2 = s -> createCoroutine(foo, &arg2);

	printf("main start\n");
	while (s -> getStatus(co1) && s -> getStatus(co2)) {
		// 使用协程co1
		s -> resume(co1);
		// 使用协程co2
		s -> resume(co2);
	} 
	printf("main end\n");
}

int  main() {
	// 创建一个协程调度器
	std::shared_ptr<CoScheduler> scheduler = std::shared_ptr<CoScheduler>(new CoScheduler());	
	test(scheduler);
	return 0;
}
