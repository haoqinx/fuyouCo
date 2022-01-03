#include "Coroutine.h"
#include <stdio.h>
#include <memory>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <iostream>

using namespace fuyou;

struct args {
	int n;
};

void foo(CoScheduler* s, void *ud) {
	struct args * arg = (struct args*)ud;
	int start = arg->n;
	int i;
	for (i=0;i<5;i++) {
		printf("coroutine %d : %d\n",s->currentCo() , start + i);
		// 切出当前协程
		s->yield();
	}
}

void test(CoScheduler* s) {
	struct args arg1 = { 0 };
	struct args arg2 = { 100 };

	// 创建两个协程
	int co1 = s -> createCoroutine(foo, (void*)&arg1);
	int co2 = s -> createCoroutine(foo, (void*)&arg2);

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
	CoScheduler* s = new CoScheduler();
	test(s);

	
	delete(s);
	return 0;
}
