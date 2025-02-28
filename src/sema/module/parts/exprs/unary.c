#include "exprs.h"
#include "sema/type/private.h"
#include "core/assert.h"
#include "sema/value/private.h"
#include "sema/const/api.h"

SemaValue *sema_analyze_expr_unary(SemaModule *sema, AstExprUnary *unary, SemaExprCtx ctx) { 
    SemaType *type = sema_value_expr_type(sema, unary->expr, ctx);
    if (!type) {
        return false;
    }
    SemaValue *value = unary->expr->value;
    switch (unary->type) {
        case AST_UNARY_MINUS:
            if (!sema_type_is_int(type) && !sema_type_is_float(type)) {
                SEMA_ERROR(ctx.loc, "unary minus operator can be applied to integer or float, not {sema::type}", type);
            }
            if (value->type == SEMA_VALUE_CONST) {
                if (sema_type_is_int(type)) {
                    return sema_value_const(sema_const_int(type, -value->constant.integer));
                }
                if (sema_type_is_float(type)) {
                    return sema_value_const(sema_const_float(type, -value->constant.fp));
                }
            }
			return sema_value_final(type);
        case AST_UNARY_BITNOT:
            if (sema_type_is_int(type)) {
                SEMA_ERROR(ctx.loc, "unary bitnot operator can be applied to integer or float, not {sema::type}", type);
            }
            if (value->type == SEMA_VALUE_CONST) {
                return sema_value_const(sema_const_int(type, ~value->constant.integer));
            }
			return sema_value_final(type);
    }
    assert(0, "invalid unary type: {int}", unary->type);
}
