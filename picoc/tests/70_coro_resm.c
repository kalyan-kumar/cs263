#include <stdio.h>
#include <stdlib.h>

float testCoroutine(int a, int b) {
	float x=0.0;
	printf("Hellyeah!!\n");
	suspend 0.0;
	printf("Noooo wayy !!!\n");
	return x;
}

int main() {
	void * h = co_await testCoroutine(5, 7);
	resume h;
	printf("Suspended. Back !!\n");
	resume h;
	printf("Resumed and back\n");
	if (h != NULL) {
		printf("Success\n");
	}
	return 0;
}
