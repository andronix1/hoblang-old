#include "value.h"
#include "sema/value.h"
#include "core/assert.h"

SemaModule *sema_value_is_module(SemaValue *value) {
    if (value->kind == SEMA_VALUE_COMP_TIME &&
            value->comp_time.kind == SEMA_VALUE_COMPTIME_MODULE) {
        return value->comp_time.module;
    }
    return NULL;
}

SemaConst *sema_value_is_const(SemaValue *value) {
    if (value->kind == SEMA_VALUE_RUNTIME &&
            value->runtime.kind == SEMA_VALUE_RUNTIME_CONST) {
        return value->runtime.constant;
    }
    return NULL; 
}

SemaType *sema_value_is_ext_func_handle(SemaValue *value) {
    if (value->kind == SEMA_VALUE_RUNTIME &&
            value->runtime.kind == SEMA_VALUE_RUNTIME_EXT_FUNC_HANDLE) {
        return value->runtime.type;
    }
    return NULL; 
}

SemaType *sema_value_is_var(SemaValue *value) {
    if (value->kind == SEMA_VALUE_RUNTIME &&
            value->runtime.kind == SEMA_VALUE_RUNTIME_VAR) {
        return value->runtime.type;
    }
    return NULL; 
}

SemaType *sema_value_is_runtime(SemaValue *value) {
    if (value->kind == SEMA_VALUE_RUNTIME) {
        return value->runtime.type;
    }
    return NULL; 
}

SemaType *sema_value_is_type(SemaValue *value) {
    if (value->kind == SEMA_VALUE_COMP_TIME &&
            value->comp_time.kind == SEMA_VALUE_COMPTIME_TYPE) {
        return value->comp_time.type;
    }
    return NULL; 
}

SemaValue *sema_runtime_value_nest_with(SemaValue *source, SemaType *type) {
    assert(sema_value_is_runtime(source), "passed non-runtime value");
    switch (source->runtime.kind) {
        case SEMA_VALUE_RUNTIME_CONST: assert(0, "const values cannot be nested");
        case SEMA_VALUE_RUNTIME_EXT_FUNC_HANDLE:
        case SEMA_VALUE_RUNTIME_FINAL:
            return sema_value_new_runtime(sema_value_runtime_new_final(type));
        case SEMA_VALUE_RUNTIME_VAR:
            return sema_value_new_runtime(sema_value_runtime_new_var(type));
    }
    assert(0, "invalid runtime value");
}
