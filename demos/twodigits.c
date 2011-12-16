#include <stdio.h>
#include "s2e.h"
int main(int argc, char **argv)
{
	int x,y;
	s2e_make_symbolic(&x, sizeof(x), "x");
	s2e_make_symbolic(&y, sizeof(y), "y");
	if (x > y) {
		printf("0 1\n");
	} else {
		printf("1 0\n");
	}
}
