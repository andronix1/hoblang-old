#include "exprs.h"

SemaValue *sema_analyze_expr_get_inner(SemaModule *sema, AstExprGetInner *get_inner, SemaExprCtx ctx) { 
    SemaType *of_type = sema_value_expr_type(sema, get_inner->of, sema_expr_ctx_expect(ctx, NULL));
    if (!of_type) {
        return NULL;
    }
    return sema_resolve_inner_value_path(sema, of_type, &get_inner->path, 0);
}
