# dot.c
`dot.c` is a toy library supporting functional programming in C99, with test codes writes in `C++11`.

## Build
You need 

+ A compiler with full C99 support
+ libtcc

to build `dot.c`. 

## Features

+ Function composition

## Examples

```c
#include <stdio.h>
#include <dot.h>

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
```

prints:
```
duo(succ(3)) = 8
duo(succ(4)) = 10
duo(succ(5)) = 12
duo(succ(6)) = 14
duo(succ(7)) = 16
duo(succ(8)) = 18
duo(succ(9)) = 20
```
