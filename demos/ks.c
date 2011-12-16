#include <s2e.h>
#include <stdio.h>

int main(int argc, char **argv)
{
  int x;
  if (argc > 1) {
    s2e_kill_state(0, argv[1]);
  }else {
    s2e_kill_state(0, "You killed me");
  }
}
