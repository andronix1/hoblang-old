#include "../../parts.h"

bool sema_analyze_expr_ref(SemaModule *sema, AstExpr *ref_expr, SemaExprCtx ctx) { 
    SemaType *type = sema_var_expr_type(sema, ref_expr, sema_expr_ctx_expect(ctx, NULL));
    if (!type) {
        return false;
    }
    return sema_value_const(ctx.value, sema_type_new_pointer(type));
}

