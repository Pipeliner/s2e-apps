#include <stdio.h>
#include "s2e.h"
int main(int argc, char **argv)
{
	int x,y;
	s2e_make_symbolic(&x, sizeof(x), "x");
	s2e_make_symbolic(&y, sizeof(y), "y");
	if (x > y) {
		s2e_message("x > y");
	} else {
		s2e_message("x <= y");
	}
}
