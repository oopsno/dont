/** @file
 * Function composition
 */
#ifndef DONT_COMPOSITION_H
#define DONT_COMPOSITION_H

#include <stdarg.h>
#include <stddef.h>

#if (defined(__cplusplus))
extern "C" {
#endif

/** \typedef void (*any_function_t)(void)
 * pointer to function (void) returning void
 */
typedef void (*any_function_t)(void);
/**
 * structure representing a composed function.
 */
typedef struct {
  any_function_t wrapped; /**< function pointer to generated code */
  void *scope;            /**< memory filled by TCC's tcc_relocate */
  void **functions;       /**< function pointers to components, always ends with NULL */
  size_t length;          /**< length of field `compose_fn_t::functions` */
} compose_fn_t;

/**
 * alloc and initialize an instance of compose_fn_t
 * @return pointer to an newly allocated `compose_fn_t` instance
 */
compose_fn_t *compose_fn_alloc();

/**
 * construct an instance of compose_fn_t
 * @param rtype return type of the composed function
 * @param itype argument type of the composed function
 * @param functions function pointers to components, always ends with NULL, like `compose_fn_t::functions`
 * @return pointer to structure representing the composed function
 */
compose_fn_t *compose_fn_construct(const char *rtype, const char *itype, void **functions);

/**
 * free an instance of compose_fn_t
 *
 * @param fn `compose_fn_t` to free
 */
void compose_fn_free(compose_fn_t *fn);

/** \def COMPOSE_NEW(rtype, itype, ...)
 * Macro to create & construct composed function
 *
 * @param rtype return type of the composed function
 * @param itype argument type of the composed function
 * @return pointer to structure representing the composed function
 * @see compose_fn_construct
 * @example example/composed.c
 */
#define COMPOSE_NEW(rtype, itype, ...) \
  (compose_fn_construct(#rtype, #itype, (void*[]){__VA_ARGS__, NULL}))

/** \def COMPOSE_CALL(rtype, itype, fn, ...)
 * Macro to call a composed function
 *
 * @param rtype return type of the composed function
 * @param itype argument type of the composed function
 * @param fn `compose_fn_t` to call
 * @example example/composed.c
 */
#define COMPOSE_CALL(rtype, itype, fn, ...) (((rtype(*)(itype))(fn->wrapped))(__VA_ARGS__))

/** \def COMPOSE_FREE(fn)
 * Macro to free a composed function
 *
 * @param fn `compose_fn_t` to free
 * @example example/composed.c
 */
#define COMPOSE_FREE(fn) \
  (compose_fn_free(fn))

#if (defined(__cplusplus))
}
#endif

#endif //DONT_COMPOSITION_H
