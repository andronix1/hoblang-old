#pragma once

typedef enum {
    SEMA_INT_8 = 1 << 0,
    SEMA_INT_16 = 1 << 1,
    SEMA_INT_32 = 1 << 2,
    SEMA_INT_64 = 1 << 3,
} SemaIntBits;

typedef enum {
    SEMA_FLOAT_32 = 1 << 0,
    SEMA_FLOAT_64 = 1 << 1,
} SemaFloatBits;

typedef struct {
    SemaIntBits ints;
    SemaFloatBits floats;
} SemaArchInfo;
