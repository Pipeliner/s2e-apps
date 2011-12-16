#include <stdio.h>
#include <s2e.h>

/**
 *  Iterative version of the fibonacci algorithm
 */
int fib1(int max)
{
   int c1=0, c2=1, temp;

   for(int i=0; i<max; i++){
      temp=c2;
      c2+=c1;
      c1=temp;
   }
   return c1;
}

/**
 *  Recursive implementation of the fibonacci algorithm
 */
int fib2(int n)
{
   if (n == 1) {
      return 1;
   }
   if (n < 1) {
      return 0;
   }

   return fib2(n-1) + fib2(n-2);
}

int main(int argc, char **argv)
{
   int fib1_result;
   int fib2_result;

   int n;

   s2e_make_symbolic(&n, sizeof(n), "n");
   if (n > 20) {
      s2e_kill_state(0, "Testing fibonacci implementations up to n=20");
   }

   s2e_disable_all_apic_interrupts();

//   for (n=-2; n<10; n++) {
      fib1_result = fib1(n);
      fib2_result = fib2(n);
//      printf("%d: %d %d\n", n, fib1_result, fib2_result);
//   }

   s2e_assert(fib1_result == fib2_result);

   s2e_kill_state(0, "Success");

   return 0;
   
}
