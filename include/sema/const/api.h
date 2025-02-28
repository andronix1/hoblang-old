#include "core/vec.h"
#include "sema/const/const.h"
#include "sema/type.h"
#include "sema/type/api.h"
#include "sema/type/private.h"

static inline SemaConst sema_const_optional_filled(SemaType *of, SemaConst *value) {
    SemaConst result = {
        .type = SEMA_CONST_OPTIONAL,
        .sema_type = sema_type_new_optional(of),
        .optional = {
            .value = value,
            .is_null = false,
        }
    };
    return result;
}

static inline SemaConst sema_const_optional_null(SemaType *of) {
    SemaConst result = {
        .type = SEMA_CONST_OPTIONAL,
        .sema_type = sema_type_new_optional(of),
        .optional = {
            .is_null = true
        }
    };
    return result;
}

static inline SemaConst sema_const_float(SemaType *type, long double value) {
    assert(sema_type_is_float(type), "passed non-float type to float const type");
    // TODO: cut
    SemaConst result = {
        .type = SEMA_CONST_FLOAT,
        .sema_type = type,
        .fp = value
    };
    return result;
}

static inline SemaConst sema_const_bool(bool value) {
    // TODO: cut
    SemaConst result = {
        .type = SEMA_CONST_INT,
        .sema_type = sema_type_primitive_bool(),
        .boolean = value
    };
    return result;
}

static inline SemaConst sema_const_int(SemaType *type, int64_t value) {
    assert(sema_type_is_int(type), "passed non-integer type to integer const type");
    // TODO: cut
    SemaConst result = {
        .type = SEMA_CONST_INT,
        .sema_type = type,
        .integer = value
    };
    return result;
}

static inline SemaConst sema_const_array(SemaType *of, SemaConst *elements) {
    SemaConst result = {
        .type = SEMA_CONST_ARRAY,
        .sema_type = sema_type_new_array(vec_len(elements), of),
        .array = elements
    };
    return result;
}
