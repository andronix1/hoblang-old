#include "sema/arch/bits/private.h"
#include "sema/arch/bits/bits.h"
#include "sema/module/module.h"
#include "sema/module/impl.h"
#include "sema/type/arch.h"
#include "sema/type/private.h"

SemaType *sema_arch_uint(SemaModule *sema) {
    SemaIntBits bits = sema->arch_info.ints;
    if (bits & SEMA_INT_64) {
        return sema_type_primitive_u64();
    } else if (bits & SEMA_INT_32) {
        return sema_type_primitive_u32();
    } else if (bits & SEMA_INT_16) {
        return sema_type_primitive_u16();
    } else {
        return sema_type_primitive_u8();
    }
}

SemaType *sema_arch_usize(SemaModule *sema) {
    return sema_arch_uint(sema);
}

SemaType *sema_arch_ptr(SemaModule *sema) {
    return sema_arch_uint(sema);
}

SemaType *sema_arch_int(SemaModule *sema) {
    SemaIntBits bits = sema->arch_info.ints;
    if (bits & SEMA_INT_64) {
        return sema_type_primitive_i64();
    } else if (bits & SEMA_INT_32) {
        return sema_type_primitive_i32();
    } else if (bits & SEMA_INT_16) {
        return sema_type_primitive_i16();
    } else {
        return sema_type_primitive_i8();
    }
}

SemaType *sema_arch_float(SemaModule *sema) {
    if (!sema->arch_info.floats) {
        return NULL;
    }
    return sema_type_primitive_f32();
}
