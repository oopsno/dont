#pragma once

#include "common.h"

#if (defined(__cplusplus))
extern "C" {
#endif

DONT_PRIVATE size_t notnull_length(void **functions);
DONT_PRIVATE void **copy_functions(void **functions, size_t length);
DONT_PRIVATE char *composition_codegen(const char *rtype, const char *itype, compose_fn_t *fn);
DONT_PRIVATE int compile(compose_fn_t *fn, const char *code, const symbol_table_t *symbols, const int symbol_counts);

#if (defined(__cplusplus))
}
#endif
