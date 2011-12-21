#include <stdio.h>
#include <stdlib.h>
#include "s2e.h"
int main(int argc, char **argv)
{
	int x,y;
	s2e_make_symbolic(&x, sizeof(x), "x");
	s2e_make_symbolic(&y, sizeof(y), "y");
	if (x > y) {
		system("argvassert 0 1\n");
	} else {
		system("argvassert 1 0\n");
	}
}
