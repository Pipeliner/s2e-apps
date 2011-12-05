#include <inttypes.h>
#include <s2e.h>

/**
 *  Computes x!
 *  If x > max, computes max!
 */
uint64_t factorial1(uint64_t x, uint64_t max) {
    uint64_t ret = 1;
    for (uint64_t i = 1; i<=x && i<=max; ++i) {
        ret = ret * i;
    }
    return ret;
}

/**
 *  Computes x!
 *  If x > max, computes max!
 */
uint64_t factorial2(uint64_t x, uint64_t max) {
    if (x > max) {
        return max;
    }

    if (x == 1) {
        return x;
    }
    return x * factorial2(x-1, max);
}

int main() {
    uint64_t x;
    uint64_t max = 10;

    //Make x symbolic
    s2e_make_symbolic(&x, sizeof(x), "x");
    s2e_enable_forking();
 
    uint64_t f1 = factorial1(x, max);
    uint64_t f2 = factorial2(x, max);

    //Check the equivalence of the two functions for each path
    s2e_assert(f1 == f2);

    //In case of success, terminate the state with the
    //appropriate message
    s2e_kill_state(0, "Success");
    return 0;
}

