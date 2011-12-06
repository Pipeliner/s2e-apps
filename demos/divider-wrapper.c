#include <stdio.h>
#include "s2e.h"
int main()
{
	int x,y;
	s2e_make_symbolic(&x, sizeof(x), "x");
	s2e_make_symbolic(&y, sizeof(y), "y");
	printf("%d %d\n");
}
