#include <s2e.h>
#include <stdio.h>
int main(int argc, char **argv)
{
	int a,b;
	a = *argv[0];
	b = *argv[1];
	s2e_assert(a>b);
}
