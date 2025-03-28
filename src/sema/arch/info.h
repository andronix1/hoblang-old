#pragma once

#include "sema/interface/type.h"
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

static inline SemaArchInfo sema_arch_info_new(SemaIntBits ints, SemaFloatBits floats) {
    SemaArchInfo result = {
        .ints = ints,
        .floats = floats
    };
    return result;
}
#define SEMA_ARCH(name) SemaType *sema_arch_##name(SemaModule *sema);

SemaType *sema_arch_info_ptr(SemaArchInfo *info);
SemaType *sema_arch_info_usize(SemaArchInfo *info);
SemaType *sema_arch_info_int(SemaArchInfo *info);
SemaType *sema_arch_info_float(SemaArchInfo *info);
