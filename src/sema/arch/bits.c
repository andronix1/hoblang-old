#include "sema/arch/bits/api.h"

SemaArchInfo sema_arch_info_new(SemaIntBits ints, SemaFloatBits floats) {
    SemaArchInfo result = {
        .ints = ints,
        .floats = floats
    };
    return result;
}

