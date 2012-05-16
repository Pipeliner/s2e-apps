#include "s2e.h"
int i;
void f1() {
	i++;
	i++;
	i++;
	s2e_message("Hi from f1()\n");
	i--;

}
int main(int argc, char **argv) {
	__asm__ __volatile__(
		"mov %%ecx, %0"
		: "=r"(argc)
	);

	s2e_message("main() start\n");
	i++;
	i++;
	i++;
	i++;
	i++;
	i++;
	if (argc > 1) {
		i++;
		i++;
		f1();
	}
	i++;
	i++;
	i++;
	i++;
	i++;
	i++;
	i++;
	s2e_message("main() end\n");

	return 0;
}

