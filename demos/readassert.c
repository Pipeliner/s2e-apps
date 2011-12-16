#include <s2e.h>
#include <stdio.h>
int main()
{
	int a,b;
	scanf("%d %d\n", &a, &b);
	s2e_assert(a>b);
}
