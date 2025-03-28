#include "null.h"
#include "sema/module.h"
#include "sema/type/type.h"
#include "sema/value.h"

SemaValue *sema_module_analyze_expr_null(SemaModule *sema, FileLocation loc, SemaExprCtx ctx) {
    if (!ctx.expectation) {
        SEMA_ERROR(loc, "failed to determine null type. Expected to expect pointer or optional");
        return NULL;
    }
    if (ctx.expectation->kind == SEMA_TYPE_POINTER ||
            ctx.expectation->kind == SEMA_TYPE_OPTIONAL) {
        return sema_value_new_runtime(sema_value_runtime_new_final(ctx.expectation));
    } else {
        SEMA_ERROR(loc, "invalid expectation for null type. Expected to expect pointer or optional but got {sema::type}", ctx.expectation);
        return NULL; 
    }
}
