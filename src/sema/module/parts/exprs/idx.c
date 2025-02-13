#include "sema/value.h"
#include "sema/module.h"
#include "sema/type/private.h"
#include "ast/private/expr/idx.h"
#include "exprs.h"

SemaValue *sema_analyze_expr_idx(SemaModule *sema, AstExprIdx *idx, SemaExprCtx ctx) {
    SemaType *of_type = sema_value_expr_type(sema, idx->of, sema_expr_ctx_expect(ctx, sema_type_new_slice(ctx.expectation)));
    if (!of_type) {
        return NULL;
    }
    if (of_type->type != SEMA_TYPE_SLICE) {
        sema_err("cannot use idx expression on non-slice expression {ast::expr}", idx->of);
        return NULL;
    }
    SemaType *idx_type = sema_value_expr_type(sema, idx->idx, sema_expr_ctx_expect(ctx, sema_type_primitive_i32()));
    if (!idx_type) {
        return NULL;
    }
    if (!sema_types_equals(idx_type, sema_type_primitive_i32())) {
        sema_err("idx value must be an i32, not {sema::type}", idx_type);
        return NULL;
    }
    return sema_value_with_type(idx->of->value, of_type->slice_of);
}