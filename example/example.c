#include <stdio.h>
#include "dot.h"

int succ(int value) {
  return value + 1;
}

int duo(int value) {
  return value + value;
}

int main() {
  typedef int (*proc_t)(int);

  compose_fn_t *fn = dot_compose("int", "int", (void*[]){duo, succ, NULL});
  int result = ((proc_t)(fn->wrapped))(3);
  printf("duo(succ(3)) = %d\n", result);
  compose_fn_delete(fn);
  return 0;
}
