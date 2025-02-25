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
    SemaType *idx_type = sema_value_expr_type(sema, idx->idx, sema_expr_ctx_expect(ctx, sema_type_primitive_i32()));
    if (!idx_type) {
        return NULL;
    }
    if (!sema_types_equals(idx_type, sema_type_primitive_i32())) {
        SEMA_ERROR(ctx.loc, "idx value must be an i32, not {sema::type}", idx_type);
        return NULL;
    }
    switch (of_type->type) {
        case SEMA_TYPE_SLICE: {
            idx->sema = SEMA_EXPR_IDX_SLICE;
            return sema_value_with_type(idx->of->value, of_type->slice_of);
        }
        case SEMA_TYPE_ARRAY: {
            idx->sema = SEMA_EXPR_IDX_ARRAY;
            return sema_value_with_type(idx->of->value, of_type->array.of);
        }
        default:
            SEMA_ERROR(ctx.loc, "cannot use idx expression on expression {ast::expr}", idx->of);
            return NULL;
    }
}
