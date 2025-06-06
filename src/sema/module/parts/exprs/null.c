#include "exprs.h"
#include "sema/module/private.h"
#include "sema/type/private.h"
#include "sema/const/api.h"
#include "sema/value/private.h"

SemaValue *sema_analyze_expr_null(SemaModule *sema, SemaNullType *type, SemaExprCtx ctx) {
    if (!ctx.expectation) {
        SEMA_ERROR(ctx.loc, "null expects that any type expected, but there is not. Specify it or use as-expression");
        return NULL;
    }
    if (ctx.expectation->type == SEMA_TYPE_POINTER) {
        *type = SEMA_NULL_POINTER;
        return sema_value_final(ctx.expectation);
    }
    if (ctx.expectation->type == SEMA_TYPE_OPTIONAL) {
        *type = SEMA_NULL_OPTIONAL;
        return sema_value_const(sema_const_optional_null(ctx.expectation->optional_of));
    }
    SEMA_ERROR(ctx.loc, "null expects that pointer or optional expected, but got {sema::type}", ctx.expectation);
    return NULL;
}
