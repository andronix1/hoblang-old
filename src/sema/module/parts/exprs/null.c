#include "exprs.h"
#include "sema/type/private.h"

SemaValue *sema_analyze_expr_null(SemaModule *sema, SemaNullType *type, SemaExprCtx ctx) {
    if (!ctx.expectation) {
        sema_err("cannot detect null type");
    }
    if (ctx.expectation->type == SEMA_TYPE_POINTER) {
        *type = SEMA_NULL_POINTER;
        return sema_value_const(ctx.expectation);
    }
    if (ctx.expectation->type == SEMA_TYPE_OPTIONAL) {
        *type = SEMA_NULL_OPTIONAL;
        return sema_value_const(ctx.expectation);
    }
    sema_err("cannot detect null type from {sema::type}", ctx.expectation);
    return NULL;
}
