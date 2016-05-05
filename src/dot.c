/** @file */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libtcc.h>
#include <dot.h>
#include "common.h"
#include "dot.h"

#if (defined(DOT_DEBUG))
#include "composed_inner.h"
#endif

#if (defined(__cplusplus))
extern "C" {
#endif

/**
 * calculate length of a given function list.
 *
 * @param functions function pointers, always ends with NULL
 * @return length of `functions`, NULL excluded
 */
DOT_PRIVATE size_t notnull_length(void **functions) {
  size_t length = 0;
  if (functions != NULL) {
    while (*functions++ != NULL) {
      length += 1;
    }
  }
  return length;
}

/**
 * Duplicate function list.
 *
 * This ensures composed functions can be constructed from stack objects safely.
 *
 * @param functions function pointers to components, always ends with NULL, like `compose_fn_t::functions`
 * @param length length of `functions`
 * @return duplication of `functions`
 */
DOT_PRIVATE void **copy_functions(void **functions, size_t length) {
  if (functions == NULL || length == 0) {
    return NULL;
  }
  void **duplication = malloc(sizeof(void *) * (length + 1));
  assert(duplication != NULL);
  for (size_t i = 0; i <= length; i++) {
    duplication[i] = functions[i];
  }
  return duplication;
}

DOT_PRIVATE compose_fn_t *compose_fn_alloc() {
  compose_fn_t *fn = (compose_fn_t *) malloc(sizeof(compose_fn_t));
  fn->wrapped = NULL;
  fn->scope = NULL;
  fn->functions = NULL;
  fn->length = 0;
  return fn;
}

void compose_fn_free(compose_fn_t *fn) {
  if (fn != NULL) {
    if (fn->scope != NULL) {
      free(fn->scope);
    }
    if (fn->functions != NULL) {
      free(fn->functions);
    }
  }
  free(fn);
}

/**
 * generate code for composed functions on the fly
 *
 * @param rtype return type of the composed function
 * @param itype argument type of the composed function
 * @param fn pointer to target `compose_fn_t` instance.
 * It's `compose_fn_t::functions` and `compose_fn_t::length` fields must be correctly set.
 * @return generated code
 */
DOT_PRIVATE char *dot_codegen(const char *rtype, const char *itype, compose_fn_t *fn) {
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

/**
 * compile a piece of code to executable binary on the fly by TCC
 *
 * @param fn pointer to target `compose_fn_t` instance.
 * It's `compose_fn_t::functions` and `compose_fn_t::length` fields must be correctly set.
 * @param code code to compile
 * @param symbols `symbol_table_t`s for extern function used in `code`
 * @param symbol_counts length of `symbols`
 * @return 0 if compile successes, -1 if error
 */
DOT_PRIVATE int compile(compose_fn_t *fn, const char *code, const symbol_table_t *symbols, const int symbol_counts) {
  TCCState *state = tcc_new();
  tcc_set_output_type(state, TCC_OUTPUT_MEMORY);
  int err = tcc_compile_string(state, code);
  if (err != 0) {
    fprintf(stderr, "Cannot compile code %s", code);
    return err;
  }
  for (int i = 0; i < symbol_counts; ++i) {
    tcc_add_symbol(state, symbols[i].name, symbols[i].address);
  }
  int scope_size = tcc_relocate(state, NULL);
  fn->scope = malloc((size_t) scope_size);
  tcc_relocate(state, fn->scope);
  fn->wrapped = tcc_get_symbol(state, "wrapped");
  tcc_delete(state);
  return 0;
}

compose_fn_t *compose_fn_construct(const char *rtype, const char *itype, void **functions) {
  compose_fn_t *fn = compose_fn_alloc();
  fn->length = notnull_length(functions);
  fn->functions = copy_functions(functions, fn->length);
  char *code = dot_codegen(rtype, itype, fn);
  compile(fn, code, NULL, 0);
  free(code);
  return fn;
}

#if (defined(__cplusplus))
}
#endif
