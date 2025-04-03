#include "value.h"
#include "const.h"

SemaValueRuntime sema_value_runtime_new_const(SemaConst *constant) {
    SemaValueRuntime result = {
        .kind = SEMA_VALUE_RUNTIME_CONST,
        .type = constant->type,
        .constant = constant
    };
    return result;
}

SemaValue *sema_value_runtime_nest(SemaValue *value, SemaType *type) {
    assert(value->kind == SEMA_VALUE_RUNTIME, "only runtime values can be nested");
    switch (value->runtime.kind) {
        case SEMA_VALUE_RUNTIME_FINAL: return sema_value_new_runtime_final(type);
        case SEMA_VALUE_RUNTIME_VAR: return sema_value_new_runtime_var(type);
        case SEMA_VALUE_RUNTIME_CONST: return sema_value_new_runtime_final(type);
        case SEMA_VALUE_RUNTIME_EXT_FUNC_HANDLE: return sema_value_new_runtime_final(type);
    }
    assert(0, "invalid runtime value");
}
