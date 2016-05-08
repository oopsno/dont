/** @file
 * Don't -- Object System, inspired by CPython
 */

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

#if (defined(__cplusplus))
extern "C" {
#endif

#define DontObject_HEADER DontObject object_header
#define $new(dtype, ...) \
  (dtype *) _dont_object_create(sizeof(dtype), (void *) _dont_##dtype##_ctor, __VA_ARGS__)

#define $size(object) \
  (((DontObject *) objcet)->size)

#define $type(object) \
  (((DontObject *) object)->type)

#define $$(object) \
  (object->value)

#define $bool(object) \
  (_dont_object_to_bool((DontObject *)object))

typedef struct _type_object_t DontTypeObject;
typedef struct _object_methods_t DontTypeObjectMethods;
typedef struct _arithmetic_methods_t DontTypeArithmeticMethods;
typedef struct _logic_methods_t DontTypeLogicMethods;

typedef struct _DontObject {
  DontTypeObject *type;
  size_t size;
} DontObject;

struct _type_object_t {
  const char *name;
  const DontTypeObjectMethods *object_methods;
  const DontTypeLogicMethods *logic_methods;
  const DontTypeArithmeticMethods *arithmetic_methods;
};

struct _object_methods_t {
  intptr_t type;
};

struct _arithmetic_methods_t {
  intptr_t add;
  intptr_t sub;
};

struct _logic_methods_t {
  intptr_t to_bool;
};

void *_dont_object_create(size_t size, void *ctor, ...);
bool _dont_object_to_bool(DontObject *object);

struct _dont_i32_type {
  DontObject_HEADER;
  int32_t value;
};

#if (defined(__cplusplus))
}
#endif
