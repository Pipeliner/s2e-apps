#include <s2e.h>
#include <stdio.h>

int solvesEq(int a1, int a2, int b1, int b2)
{
    // 3, 20, 2, 80..87
    int x = a1*a2;
    int y = a1+a2;
    int z = b1*b1*b1*b1*b1*b1*a1+4;
    int w = b2 >> 3 + a2;
    return (x==60 && y==23 && z==196 && w==42 );
}

int main()
{
   unsigned a1, a2, b1, b2;

   s2e_make_symbolic(&a1, sizeof(a1), "a1");
   s2e_make_symbolic(&a2, sizeof(a2), "a2");
   s2e_make_symbolic(&b1, sizeof(b1), "b1");
   s2e_make_symbolic(&b2, sizeof(b2), "b2");

   //Just to make it a bit faster
   s2e_disable_all_apic_interrupts();

   s2e_assert(solvesEq(a1,a2,b1,b2));
   s2e_get_example(&a1, sizeof(a1));
   s2e_get_example(&a2, sizeof(a2));
   s2e_get_example(&b1, sizeof(b1));
   s2e_get_example(&b2, sizeof(b2));

   s2e_print_expression("SOLUTION a1", a1);
   s2e_print_expression("a2", a2);
   s2e_print_expression("b1", b1);
   s2e_print_expression("b2", b2);

   printf("(%d %d %d %d) solves our task", a1, a2, b1, b2);

   s2e_kill_state(0, "This execution path is correct (assertion check passed)");
   return 0;
}
