#include <stdio.h>

#define PANIC(fmt, ...) \
    do { \
        fprintf(stderr, fmt"\n", ##__VA_ARGS__); \
        exit(1); \
    } while (0)

#define ASSERT(ok, fmt, ...) \
    do { \
        if (!(ok)) { \
            PANIC("Assertion failed: " fmt, ##__VA_ARGS__); \
        } \
    } while (0)

#define ASSERT_EQ(src, other) ASSERT(src == other, #src " != " #other)
#define ASSERT_EQ_DEEP(src, other, func) ASSERT(func(src, other), #src " != " #other)

#define OK(what) printf("ok: " what "\n")

