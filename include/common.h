#pragma once

#if (!defined(NDEBUG) && !defined(DONT_DEBUG))
#define DONT_DEBUG
#endif

#if (defined(DONT_DEBUG))
#define DONT_PRIVATE
#define DONT_PRIVATE_ENABLED 0
#else
#define DONT_PRIVATE static
#define DONT_PRIVATE_ENABLED 1
#endif

/**
 * structure to record external symbols
 */
typedef struct {
  const char *name; /**< the name of the symbol(function) */
  void *address;    /**< the address of the symbol */
} symbol_table_t;
