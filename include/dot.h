#ifndef DOT_C_DOT_H
#define DOT_C_DOT_H

#include <stdarg.h>
#include <stddef.h>

typedef struct {
  void *wrapped;
  void **functions;
  size_t length;
} compose_fn_t;

compose_fn_t *dot_compose(const char * rtype, const char * itype, void **functions);

void compose_fn_delete(compose_fn_t *);

compose_fn_t *compose_fn_new();

#define STRUCTUREP_TO_FNP(sp) (sp->wrapped)

#define FNP_TO_STRUCTUREP(fp) ((compose_fn_t*)fp)

#define COMPOSEFN(rtype, itype, fn) \
  (((rtype*)(itype)) STRUCTUREP_TO_FNP(dot_compose(fn)))

#endif //DOT_C_DOT_H
