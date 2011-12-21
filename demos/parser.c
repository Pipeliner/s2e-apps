#include <stdio.h>

void sayA() {
	printf("A\n");
}

void sayB() {
	printf("B\n");
}
int main() {
	void (*table[])(void) = {sayA, sayB};
	char buf[40];
	FILE *f = fopen("/mnt/symbolic", "r");
	if (!f)
		return 1;
	fgets(buf, 400, f);
	table[buf[0]]();
}
