#include <gtest/gtest.h>
#include "dot.h"
#include "composed_inner.h"

TEST(Composed, notnull_length) {
  EXPECT_EQ(notnull_length(NULL), 0);
  void *voidptr = reinterpret_cast<void *>(notnull_length);
  EXPECT_EQ(notnull_length((void *[]) {voidptr, voidptr, voidptr, nullptr}), 3);
  EXPECT_EQ(notnull_length((void *[]) {voidptr, voidptr, nullptr, voidptr}), 2);
}

TEST(Composed, copy_functions) {
  void *pointers[] = {
      reinterpret_cast<void *>(compose_fn_alloc),
      reinterpret_cast<void *>(compose_fn_construct),
      reinterpret_cast<void *>(compose_fn_free),
      nullptr
  };
  void **duplicate = copy_functions(pointers, 3);
  EXPECT_NE(duplicate, pointers);
  for (int i = 0; i < 4; ++i) {
    EXPECT_EQ(pointers[i], duplicate[i]);
  }
  EXPECT_EQ(copy_functions(nullptr, 4), nullptr);
  EXPECT_NO_FATAL_FAILURE(free(duplicate));
};

TEST(Composed, codegen) {
  compose_fn_t *fn = compose_fn_alloc();
  char *code = dot_codegen("int", "int", fn);
  free(code);
  compose_fn_free(fn);
}

TEST(Composed, compile) {
  compose_fn_t *fn = compose_fn_alloc();
  char *code = dot_codegen("int", "int", fn);
  EXPECT_EQ(compile(fn, code, nullptr, 0), 0);
  EXPECT_NE(fn->scope, nullptr);
  EXPECT_NE(fn->wrapped, nullptr);
  free(code);
  compose_fn_free(fn);
}

TEST(Composed, compose_fn_alloc) {
  compose_fn_t *fn = nullptr;
  EXPECT_NO_FATAL_FAILURE(fn = compose_fn_alloc());
  EXPECT_NE(fn, nullptr);
  EXPECT_NO_FATAL_FAILURE(compose_fn_free(fn));
}

TEST(Composed, compose_fn_construct) {
  compose_fn_t *fn = compose_fn_construct("int", "int", nullptr);
  EXPECT_EQ(COMPOSE_CALL(int, int, fn, 3), 3);
  COMPOSE_FREE(fn);
}

TEST(Composed, compose_fn_free) {
  EXPECT_NO_FATAL_FAILURE(compose_fn_free(nullptr));
  EXPECT_NO_FATAL_FAILURE(compose_fn_free(compose_fn_alloc()));
}