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
