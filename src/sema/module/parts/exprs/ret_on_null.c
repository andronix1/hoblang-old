#include "exprs.h"
#include "sema/module/parts/expr.h"
#include "sema/module/private.h"
#include "sema/type/private.h"
#include "sema/value/private.h"

SemaValue *sema_analyze_expr_ret_on_null(SemaModule *sema, AstExprRetOnNull *ret_on_null, SemaExprCtx ctx) {
    SemaType *returns = ret_on_null->fret = sema_module_returns(sema);
    if (returns->type != SEMA_TYPE_OPTIONAL) {
        sema_err("function must returns optional, not {sema::expr}", returns);
    }
    SemaType *expr_type = sema_value_expr_type(sema, ret_on_null->expr, ctx.expectation ? sema_expr_ctx_default_of(sema_type_new_optional(ctx.expectation)) : ctx);
    if (!expr_type) {
        return NULL;
    }
    if (expr_type->type != SEMA_TYPE_OPTIONAL) {
        sema_err("ret on null's inner expression must be optional, not {sema::type}", expr_type);
    }
    return sema_value_with_type(ret_on_null->expr->value, expr_type->optional_of);
}
