#include "call.h"
#include "core/not_null.h"
#include "core/vec.h"
#include "sema/interface/type.h"
#include "sema/interface/value.h"
#include "sema/module.h"
#include "sema/type/type.h"
#include "sema/nodes/shared/expr.h"
#include "ast/shared/expr.h"
#include "sema/value.h"
#include <stddef.h>

SemaValue *sema_module_analyze_expr_call(SemaModule *sema, AstExprCall *call) {
    SemaValue *inner_value = NOT_NULL(sema_analyze_expr(sema, call->callable, sema_expr_ctx_new(NULL)));
    SemaType *type = sema_value_is_runtime(inner_value);
    if (!type) {
        SEMA_ERROR(call->callable->loc, "inner value must be runtime");
        return NULL;
    }
    if (type->kind != SEMA_TYPE_FUNCTION) {
        SEMA_ERROR(call->callable->loc, "callable expression must be function");
        return NULL;
    }
    size_t len = vec_len(type->func.args);
    if (vec_len(call->args) < len) {
        len = vec_len(call->args);
    }
    for (size_t i = 0; i < len; i++) {
        SemaValue *inner_value = sema_analyze_expr(sema, call->args[i], sema_expr_ctx_new(type->func.args[i]));
        if (!inner_value) {
            continue;
        }
        SemaType *arg_type = sema_value_is_runtime(inner_value);
        if (!arg_type) {
            SEMA_ERROR(call->args[i]->loc, "call parameters must be runtime");
            continue;
        }
        if (!sema_types_equals(arg_type, type->func.args[i])) {
            SEMA_ERROR(call->args[i]->loc, "expected call argument to be {sema::type} but got {sema::type}", arg_type, type->func.args[i]);
            continue;
        }
    }
    return sema_value_new_runtime(sema_value_runtime_new_final(type->func.returns));
}
