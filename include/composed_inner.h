#ifndef DOT_C_COMPOSED_INNER_H
#define DOT_C_COMPOSED_INNER_H

#include "common.h"

#if (defined(__cplusplus))
extern "C" {
#endif

DOT_PRIVATE size_t notnull_length(void **functions);
DOT_PRIVATE void **copy_functions(void **functions, size_t length);
DOT_PRIVATE char *dot_codegen(const char *rtype, const char *itype, compose_fn_t *fn);
DOT_PRIVATE int compile(compose_fn_t *fn, const char *code, const symbol_table_t *symbols, const int symbol_counts);

#if (defined(__cplusplus))
}
#endif

#endif //DOT_C_COMPOSED_INNER_H
