#include <stdio.h>
#include <stdlib.h>

float testCoroutine() {
	float x=0.0;
	return x;
}

int main() {
	void * h = co_await testCoroutine();
	if (h != NULL) {
		printf("Success\n");
	}
	return 0;
}
