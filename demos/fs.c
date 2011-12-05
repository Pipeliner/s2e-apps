#include <s2e.h>
#include <stdio.h>

int main()
{
  int x;
  s2e_make_symbolic(&x, sizeof(x), "x");

  if (x == 0) {
    return s2e_get_path_id(); 
  }else {
    return s2e_get_path_id();
  }
}
