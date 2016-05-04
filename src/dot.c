#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libtcc.h>
#include "dot.h"

static size_t notnull_length(void **array) {
  size_t length = 0;
  while (array != NULL && *array++ != NULL) {
    length += 1;
  }
  return length;
}

static void **copy_functions(void **functions, size_t count) {
  if (functions == NULL || count == 0) {
    return NULL;
  }
  void **duplication = malloc(sizeof(void *) * (count + 1));
  assert(duplication != NULL);
  for (size_t i = 0; i <= count; i++) {
    duplication[i] = functions[i];
  }
  return duplication;
}

compose_fn_t *compose_fn_new() {
  return malloc(sizeof(compose_fn_t));
}

void compose_fn_delete(compose_fn_t *fn) {
  free(fn->wrapped);
  free(fn->functions);
  free(fn);
}

static const char *dot_codegen(const char *rtype, const char *itype, compose_fn_t *fn) {
  assert(strcmp(rtype, itype) == 0);
  const char *template = ""
      "typedef %s rtype;"
      "typedef %s itype;"
      "typedef rtype (*fn_t)(itype);"
      "rtype wrapped(itype value) {"
      "  const fn_t* functions = (fn_t*) %pULL;"
      "  const int length = %d;"
      "  itype iter = value;"
      "  for (int i = length - 1; i >= 0; i--) {"
      "    iter = functions[i](iter);"
      "  }"
      "  return iter;"
      "}";
  char *rtv;
  asprintf(&rtv, template, rtype, itype, fn->functions, fn->length);
  if (rtv == NULL) {
    fprintf(stderr, "Cannot format code template.\n");
    exit(-1);
  }
  return rtv;
}

typedef struct {
  const char *name;
  void *address;
} symbol_table_t;

static void *compile(const char *code, const symbol_table_t *symbols, const int symbol_counts) {
  TCCState *state = tcc_new();
  tcc_set_output_type(state, TCC_OUTPUT_MEMORY);
  int err = tcc_compile_string(state, code);
  if (err != 0) {
    fprintf(stderr, "Cannot compile code %s", code);
    exit(-1);
  }
  for (int i = 0; i < symbol_counts; ++i) {
    tcc_add_symbol(state, symbols[i].name, symbols[i].address);
  }
  int func_size = tcc_relocate(state, NULL);
  void *func_mem = malloc((size_t) func_size);
  tcc_relocate(state, func_mem);
  void *func = tcc_get_symbol(state, "wrapped");
  tcc_delete(state);
  return func;
}

compose_fn_t *dot_compose(const char *rtype, const char *itype, void **functions) {
  compose_fn_t *fn = compose_fn_new();
  fn->length = notnull_length(functions);
  fn->functions = copy_functions(functions, fn->length);
  const char *code = dot_codegen(rtype, itype, fn);
  fn->wrapped = compile(code, NULL, 0);
  return fn;
}