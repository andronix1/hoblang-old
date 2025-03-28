#include "exprs.h"
#include "sema/module/parts/path.h"

SemaValue *sema_analyze_expr_get_inner(SemaModule *sema, AstExprGetInner *get_inner, SemaExprCtx ctx) { 
    SemaValue *value = sema_expr(sema, get_inner->of, sema_expr_ctx_expect(ctx, NULL));
    if (!value) {
        return NULL;
    }
    return sema_resolve_path(sema, value, get_inner->path);
}
