#include "info.h"
#include "sema/type/type.h"

SemaType *sema_arch_info_uint(SemaArchInfo *info) {
    SemaIntBits bits = info->ints;
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

SemaType *sema_arch_info_usize(SemaArchInfo *info) {
    return sema_arch_info_uint(info);
}

SemaType *sema_arch_info_ptr(SemaArchInfo *info) {
    return sema_arch_info_uint(info);
}

SemaType *sema_arch_info_int(SemaArchInfo *info) {
    SemaIntBits bits = info->ints;
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

SemaType *sema_arch_info_float(SemaArchInfo *info) {
    if (!info->floats) {
        return NULL;
    }
    return sema_type_primitive_f32();
}
