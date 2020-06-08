#include <stdio.h>
#include <stdlib.h>

float testCoroutine(int a, int b) {
	float x=0.0;
	printf("Suspending once!!\n");
	suspend 0.0;
	printf("Returning now\n");
	return x;
}

int main() {
	void * h = co_await testCoroutine(5, 7);
	printf("Coroutine created\n");
	resume h;
	printf("Suspended!! Resuming again\n");
	resume h;
	printf("Coroutine finished execution\n");
	printf("Success\n");
	return 0;
}
