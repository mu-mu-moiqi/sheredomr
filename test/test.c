#include "hashmap.h"
#include "utest.h"

UTEST(c, create) {
  struct hashmap_s hashmap;
  ASSERT_EQ(0, hashmap_create(1, &hashmap));
  hashmap_destroy(&hashmap);
}

static int set_context(void *const context, void *const element) {
  *(int *)context = *(int *)element;
  return 1;
}

UTEST(c, put) {
  struct hashmap_s hashmap;
  int x = 42;
  int y = 13;
  ASSERT_EQ(0, hashmap_create(1, &hashmap));
  ASSERT_EQ(0, hashmap_put(&hashmap, "foo", (unsigned)strlen("foo"), &x));
  ASSERT_EQ(0, hashmap_iterate(&hashmap, set_context, &y));
  ASSERT_EQ(x, y);
  hashmap_destroy(&hashmap);
}

UTEST(c, get_exists) {
  struct hashmap_s hashmap;
  int x = 42;
  ASSERT_EQ(0, hashmap_create(1, &hashmap));
  ASSERT_EQ(0, hashmap_put(&hashmap, "foo", (unsigned)strlen("foo"), &x));
  ASSERT_EQ(&x, hashmap_get(&hashmap, "foo", (unsigned)strlen("foo")));
  hashmap_destroy(&hashmap);
}

UTEST(c, get_does_not_exists) {
  struct hashmap_s hashmap;
  ASSERT_EQ(0, hashmap_create(1, &hashmap));
  ASSERT_EQ(NULL, hashmap_get(&hashmap, "foo", (unsigned)strlen("foo")));
  hashmap_destroy(&hashmap);
}

UTEST(c, remove) {
  struct hashmap_s hashmap;
  int x = 42;
  ASSERT_EQ(0, hashmap_create(1, &hashmap));
  ASSERT_EQ(0, hashmap_put(&hashmap, "foo", (unsigned)strlen("foo"), &x));
  ASSERT_EQ(0, hashmap_remove(&hashmap, "foo", (unsigned)strlen("foo")));
  hashmap_destroy(&hashmap);
}

static int early_exit(void *const context, void *const element) {
  *(int *)context += 1;
  *(int *)element += 1;
  return 0;
}

UTEST(c, iterate_early_exit) {
  struct hashmap_s hashmap;
  int x[27] = {0};
  int total = 0;
  char s[27];
  char c;
  ASSERT_EQ(0, hashmap_create(1, &hashmap));

  for (c = 'a'; c <= 'z'; c++) {
    s[c - 'a'] = c;
  }

  for (c = 'a'; c <= 'z'; c++) {
    const int index = c - 'a';
    ASSERT_EQ(0, hashmap_put(&hashmap, s + index, 1, x + index));
  }

  ASSERT_EQ(1, hashmap_iterate(&hashmap, early_exit, &total));
  ASSERT_EQ(1, total);

  total = 0;

  for (c = 'a'; c <= 'z'; c++) {
    const int index = c - 'a';
    ASSERT_GE(1, x[index]);
    if (x[index]) {
      total += 1;
    }
  }

  ASSERT_EQ(1, total);

  hashmap_destroy(&hashmap);
}

static int all(void *const context, void *const element) {
  *(int *)context += 1;
  *(int *)element += 1;
  return 1;
}

UTEST(c, iterate_all) {
  struct hashmap_s hashmap;
  int x[27] = {0};
  int total = 0;
  char s[27];
  char c;
  ASSERT_EQ(0, hashmap_create(1, &hashmap));

  for (c = 'a'; c <= 'z'; c++) {
    s[c - 'a'] = c;
  }

  for (c = 'a'; c <= 'z'; c++) {
    const int index = c - 'a';
    ASSERT_EQ(0, hashmap_put(&hashmap, s + index, 1, x + index));
  }

  ASSERT_EQ(0, hashmap_iterate(&hashmap, all, &total));
  ASSERT_EQ(26, total);

  for (c = 'a'; c <= 'z'; c++) {
    const int index = c - 'a';
    ASSERT_EQ(1, x[index]);
  }

  hashmap_destroy(&hashmap);
}

UTEST(c, size) {
  struct hashmap_s hashmap;
  int x = 42;
  ASSERT_EQ(0, hashmap_create(1, &hashmap));
  ASSERT_EQ(0u, hashmap_size(&hashmap));
  ASSERT_EQ(0, hashmap_put(&hashmap, "foo", (unsigned)strlen("foo"), &x));
  ASSERT_EQ(1u, hashmap_size(&hashmap));
  ASSERT_EQ(0, hashmap_remove(&hashmap, "foo", (unsigned)strlen("foo")));
  ASSERT_EQ(0u, hashmap_size(&hashmap));
  hashmap_destroy(&hashmap);
}