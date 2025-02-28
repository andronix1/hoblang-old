#include "exprs.h"
#include "sema/type/private.h"
#include "sema/value/private.h"
#include "sema/const/api.h"

SemaValue *sema_analyze_expr_not(SemaModule *sema, AstExpr *not_expr, SemaExprCtx ctx) { 
    SemaType *type = sema_value_expr_type(sema, not_expr, sema_expr_ctx_expect(ctx, sema_type_primitive_bool()));
    if (!sema_types_equals(type, sema_type_primitive_bool())) {
        SEMA_ERROR(ctx.loc, "not operator can be only be applied to booleans, not {sema::type}", type);
    }
    if (not_expr->value->type == SEMA_VALUE_CONST) {
        return sema_value_const(
            sema_const_bool(!not_expr->value->constant.boolean)
        );
    }
    return sema_value_final(sema_type_primitive_bool());
}
