#ifndef DOT_C_DOT_H
#define DOT_C_DOT_H

#include <stdarg.h>
#include <stddef.h>

typedef void (*any_function_t)(void);

typedef struct {
  any_function_t wrapped;
  void *scope;
  void **functions;
  size_t length;
} compose_fn_t;

compose_fn_t *dot_compose(const char *rtype, const char *itype, void **functions);

void compose_fn_delete(compose_fn_t *);

compose_fn_t *compose_fn_new();

#define COMPOSE_NEW(rtype, itype, ...) \
  (dot_compose(#rtype, #itype, (void*[]){__VA_ARGS__, NULL}))

#define COMPOSE_CALL(rtype, itype, sp, ...) (((rtype(*)(itype))(sp->wrapped))(__VA_ARGS__))

#define COMPOSE_FREE(fp) \
  (compose_fn_delete(fp))

#endif //DOT_C_DOT_H
