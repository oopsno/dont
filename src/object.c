#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include "object.h"

#if (defined(__cplusplus))
extern "C" {
#endif

typedef void (*fn_t)(void *, va_list);

void *_dont_object_create(size_t size, void *ctor, ...) {
  void *object = malloc(size);
  va_list args;
  va_start(args, ctor);
  ((fn_t) ctor)(object, args);
  va_end(args);
  return object;
}

bool _dont_object_to_bool(DontObject *object) {
  return ((bool (*)(DontObject *)) ($type(object)->methods->to_bool))(object);
}

#if (defined(__cplusplus))
}
#endif