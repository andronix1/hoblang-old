#include "exprs.h"

SemaValue *sema_analyze_expr_not(SemaModule *sema, AstExpr *not_expr, SemaExprCtx ctx) { 
    SemaType *type = sema_value_expr_type(sema, not_expr, sema_expr_ctx_expect(ctx, sema_type_primitive_bool()));
    if (!sema_types_equals(type, sema_type_primitive_bool())) {
        sema_err("not operator can be only be applied to booleans, not {sema::type}", type);
    }
    return sema_value_const(sema_type_primitive_bool());
}
