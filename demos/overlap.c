/**
 *  This demo implements two versions of an algorithm that is supposed to
 *  check whether two integer ranges overlap.
 *  One of the functions (or maybe both) are buggy and disagree on some conditions.
 *  Symbolic execution quickly finds the problem.  
 */

#include <s2e.h>
#include <stdio.h>

int correct_overlap(unsigned a1, unsigned a2, unsigned b1, unsigned b2)
{
   if (!(a1 < a2 || b1 < b2)) {
     return -1;
   }

   if (b1 > a2) {
      return 0;
   }
   if (b2 < a1) {
      return 0;
   }

   if (b1 <= a1 && b2 <= a2) {
      return 1;
   }

   return 0;

}

int student_overlap(unsigned a1, unsigned a2, unsigned b1, unsigned b2)
{
   if (!(a1 < a2 || b1 < b2)) {
     return -1;
   }

   if (b1 > a2) {
      return 0;
   }
   if (b2 < a1) {
      return 0;
   }
   return 1;
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

   s2e_assert(correct_overlap(a1, a2, b1, b2) == student_overlap(a1, a2, b1, b2));

   s2e_kill_state(0, "This execution path is correct (assertion check passed)");
   return 0;
}
