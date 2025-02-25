#include "exprs.h"
#include "sema/type/private.h"
#include "core/assert.h"

SemaValue *sema_analyze_expr_unary(SemaModule *sema, AstExprUnary *unary, SemaExprCtx ctx) { 
    SemaType *type = sema_value_expr_type(sema, unary->expr, ctx);
    if (!type) {
        return false;
    }
    switch (unary->type) {
        case AST_UNARY_MINUS:
            if (type->type != SEMA_TYPE_PRIMITIVE || (
                type->primitive.type != SEMA_PRIMITIVE_FLOAT &&
                type->primitive.type != SEMA_PRIMITIVE_INT
            )) {
                SEMA_ERROR(ctx.loc, "unary minus operator can be applied to integer or float, not {sema::type}", type);
            }
			return sema_value_const(type);
        case AST_UNARY_BITNOT:
            if (type->type != SEMA_TYPE_PRIMITIVE || (
                type->primitive.type != SEMA_PRIMITIVE_FLOAT &&
                type->primitive.type != SEMA_PRIMITIVE_INT
            )) {
                SEMA_ERROR(ctx.loc, "unary bitnot operator can be applied to integer or float, not {sema::type}", type);
            }
			return sema_value_const(type);
    }
    assert(0, "invalid unary type: {int}", unary->type);
}
