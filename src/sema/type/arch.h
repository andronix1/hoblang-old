#include "type.h"

SemaType *sema_type_primitive_i16() {
    static SemaType result = {
        .kind = SEMA_TYPE_PRIMITIVE,
        .primitive = {
            .kind = SEMA_PRIMITIVE_INT,
            .integer = SEMA_PRIMITIVE_INT16
        }
    };
    return &result;
}

SemaType *sema_type_primitive_i32() {
    static SemaType result = {
        .kind = SEMA_TYPE_PRIMITIVE,
        .primitive = {
            .kind = SEMA_PRIMITIVE_INT,
            .integer = SEMA_PRIMITIVE_INT32
        }
    };
    return &result;
}

SemaType *sema_type_primitive_i64() {
    static SemaType result = {
        .kind = SEMA_TYPE_PRIMITIVE,
        .primitive = {
            .kind = SEMA_PRIMITIVE_INT,
            .integer = SEMA_PRIMITIVE_INT64
        }
    };
    return &result;
}

SemaType *sema_type_primitive_u16() {
    static SemaType result = {
        .kind = SEMA_TYPE_PRIMITIVE,
        .primitive = {
            .kind = SEMA_PRIMITIVE_INT,
            .integer = SEMA_PRIMITIVE_UINT16
        }
    };
    return &result;
}

SemaType *sema_type_primitive_u32() {
    static SemaType result = {
        .kind = SEMA_TYPE_PRIMITIVE,
        .primitive = {
            .kind = SEMA_PRIMITIVE_INT,
            .integer = SEMA_PRIMITIVE_UINT32
        }
    };
    return &result;
}

SemaType *sema_type_primitive_u64() {
    static SemaType result = {
        .kind = SEMA_TYPE_PRIMITIVE,
        .primitive = {
            .kind = SEMA_PRIMITIVE_INT,
            .integer = SEMA_PRIMITIVE_UINT64
        }
    };
    return &result;
}

