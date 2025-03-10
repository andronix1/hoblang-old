#include "sema/value/api.h"
#include "sema/value/private.h"
#include "core/assert.h"

SemaType *sema_value_typeof(SemaValue *value) {
    switch (value->type) {
        case SEMA_VALUE_TYPE:
        case SEMA_VALUE_VAR:
        case SEMA_VALUE_FINAL:
        case SEMA_VALUE_EXT_FUNC_HANDLE:
            assert(value->sema_type, "type was not mapped to value");
            return value->sema_type;
        case SEMA_VALUE_CONST:
            assert(value->constant.sema_type, "type was not mapped to value");
            return value->constant.sema_type;
        case SEMA_VALUE_MODULE:
            break;
    }
    assert(0, "trying to get type of non-typed value");
}

SemaModule *sema_value_module_of(SemaValue *value) {
    if (value->type != SEMA_VALUE_MODULE) {
        return NULL;
    }
    return value->module;
}

bool sema_value_is_type_of(SemaValue *value, SemaTypeKind kind) {
    return value->type == SEMA_VALUE_TYPE && value->sema_type->type == kind;
}

inline bool sema_value_is_final(SemaValue *value) {
    return value->type == SEMA_VALUE_FINAL;
}

inline bool sema_value_is_var(SemaValue *value) {
    return value->type == SEMA_VALUE_VAR;
}

inline bool sema_value_is_const(SemaValue *value) {
    return value->type == SEMA_VALUE_CONST;
}

inline bool sema_value_is_not_mut(SemaValue *value) {
    return sema_value_is_const(value) ||
        value->type == SEMA_VALUE_FINAL;
}

inline bool sema_value_is_value(SemaValue *value) {
    return sema_value_is_not_mut(value) ||
        value->type == SEMA_VALUE_VAR;

}

bool sema_value_is_runtime(SemaValue *value) {
    return value->type == SEMA_VALUE_VAR ||
        value->type == SEMA_VALUE_FINAL;
}
