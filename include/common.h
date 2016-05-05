#ifndef DOT_C_COMMON_H
#define DOT_C_COMMON_H

#if (!defined(NDEBUG) && !defined(DOT_DEBUG))
#define DOT_DEBUG
#endif

#if (defined(DOT_DEBUG))
#define DOT_PRIVATE
#define DOT_PRIVATE_ENABLED 0
#else
#define DOT_PRIVATE static
#define DOT_PRIVATE_ENABLED 1
#endif

/**
 * structure to record external symbols
 */
typedef struct {
  const char *name; /**< the name of the symbol(function) */
  void *address;    /**< the address of the symbol */
} symbol_table_t;

#endif //DOT_C_COMMON_H
