#include "exprs.h"
#include "sema/module/parts/path.h"

SemaValue *sema_analyze_expr_get_inner(SemaModule *sema, AstExprGetInner *get_inner, SemaExprCtx ctx) { 
    SemaType *of_type = sema_value_expr_type(sema, get_inner->of, sema_expr_ctx_expect(ctx, NULL));
    if (!of_type) {
        return NULL;
    }
    return sema_resolve_inner_path(sema, get_inner->of->value, &get_inner->path);
}
