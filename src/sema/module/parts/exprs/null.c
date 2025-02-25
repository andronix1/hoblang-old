#include "exprs.h"
#include "sema/module/private.h"
#include "sema/type/private.h"

SemaValue *sema_analyze_expr_null(SemaModule *sema, SemaNullType *type, SemaExprCtx ctx) {
    if (!ctx.expectation) {
        SEMA_ERROR(ctx.loc, "null expects that any type expected, but there is not. Specify it or use as expression");
    }
    if (ctx.expectation->type == SEMA_TYPE_POINTER) {
        *type = SEMA_NULL_POINTER;
        return sema_value_const(ctx.expectation);
    }
    if (ctx.expectation->type == SEMA_TYPE_OPTIONAL) {
        *type = SEMA_NULL_OPTIONAL;
        return sema_value_const(ctx.expectation);
    }
    SEMA_ERROR(ctx.loc, "null expects that pointer or optional expected, but got {sema::type}", ctx.expectation);
    return NULL;
}
