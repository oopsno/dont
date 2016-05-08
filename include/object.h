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

#define $$(object) (object->value)
#define $call(object, method, ...) (((void *(*)(...))($type(object)->methods->method))(object, __VA_ARGS__))
#define $new(dtype, ...) DOBJ_CTOR(dtype) (__VA_ARGS__)
#define $size(object) (((DontObject *) object)->size)
#define $type(object) (((DontObject *) object)->type)
#define $bool(object) (_dont_object_to_bool((DontObject *)object))

#define DOBJ_STRUCTURE(Type) _dont_##Type##_struct
#define DOBJ_CTOR(Type) _dont_##Type##_ctor
#define DOBJ_CTOR_DEF(Type) Type *_dont_##Type##_ctor(Type *self, va_list args)
#define DOBJ_TYPEOBJ(Type) _dont_##Type##_type_obj
#define DOBJ_METHODS_STT(Type) _dont_##Type##_basic_methods
#define DOBJ_METHOD(Type, n) _dont_##Type##_basic_methods_##n
#define DOBJ_METHOD_DEF_SELF(Type, n) DontObject *DOBJ_METHOD(Type, n) (Type *self)
#define DOBJ_METHOD_DEF_BIN(Type, n) DontObject *DOBJ_METHOD(Type, n) (Type *lhs, Type *rhs)

typedef struct _type_object_t DontTypeObject;
typedef struct {
  void *to_bool;
  void *add, *sub, *mul, *div, *mod;
} DontTypeBasicMethods;

typedef struct _DontObject {
  DontTypeObject *type;
  size_t size;
} DontObject;

struct _type_object_t {
  const char *name;
  const DontTypeBasicMethods *methods;
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
