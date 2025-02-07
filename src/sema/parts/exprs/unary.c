#include "../../parts.h"

SemaValue *sema_analyze_expr_unary(SemaModule *sema, AstExprUnary *unary, SemaExprCtx ctx) { 
    SemaType *type = sema_value_expr_type(sema, unary->expr, ctx);
    if (!type) {
        return false;
    }
    switch (unary->type) {
        case AST_UNARY_MINUS:
            if (type->type != SEMA_TYPE_PRIMITIVE) {
                sema_err("cannot apply unary minus to non-primitive types");
            }
			return sema_value_const(type);
        case AST_UNARY_BITNOT:
            if (type->type != SEMA_TYPE_PRIMITIVE) {
                sema_err("cannot apply unary bitnot to non-primitive types");
            }
			return sema_value_const(type);
    }
    assert(0, "invalid unary type: {int}", unary->type);
}
