#include "ret_on_null.h"
#include "core/not_null.h"
#include "ast/shared/expr.h"
#include "sema/interface/type.h"
#include "sema/interface/value.h"
#include "sema/module.h"
#include "sema/nodes/shared/expr.h"
#include "sema/type/type.h"
#include "sema/value.h"
#include <stddef.h>

SemaValue *sema_module_analyze_expr_ret_on_null(SemaModule *sema, AstExpr *inner) {
    SemaType *type = sema_value_is_runtime(NOT_NULL(sema_analyze_expr(sema, inner, sema_expr_ctx_new(NULL))));
    if (!type) {
        SEMA_ERROR(inner->loc, "value to unwrap must be runtime");
        return NULL;
    }
    if (type->kind != SEMA_TYPE_OPTIONAL) {
        SEMA_ERROR(inner->loc, "only optionals can be unwrapped, not {sema::type}", type);
        return NULL;
    }
    SemaType *returns = sema_module_returns(sema);
    if (!returns) {
        SEMA_ERROR(inner->loc, "cannot use ret-on-null operation outside of function");
    } else if (returns->kind != SEMA_TYPE_OPTIONAL) {
        SEMA_ERROR(inner->loc, "if you wanna use ret-on-null operator, function must return optional, not {sema::type}", returns); 
    }
    return sema_value_new_runtime(sema_value_runtime_new_final(type->optional_of));
}
