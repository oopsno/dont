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

  for (int value = 3; value < 10; ++value) {
    compose_fn_t* fn = COMPOSE_NEW(int, int, duo, succ);
    printf("duo(succ(%d)) = %d\n", value, COMPOSE_CALL(int, int, fn, value));
    COMPOSE_FREE(fn);
  }
  return 0;
}
