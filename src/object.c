#include <stdlib.h>
#include <stdbool.h>
#include "object.h"

#if (defined(__cplusplus))
extern "C" {
#endif

bool _dont_object_to_bool(DontObject *object) {
  return ((bool (*)(DontObject *)) ($type(object)->methods->to_bool))(object);
}

void _dont_object_delete(DontObject *object) {
  _dobj_dtor_t dtor = $dtor(object);
  if (dtor) {
    dtor(object);
  }
  free(object);
}

#if (defined(__cplusplus))
}
#endif