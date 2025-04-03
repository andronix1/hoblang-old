#include "expr.h"
#include "ast/shared/expr.h"
#include "core/location.h"
#include "core/not_null.h"
#include "sema/arch/info.h"
#include "sema/const.h"
#include "sema/interface/type.h"
#include "sema/interface/value.h"
#include "sema/module.h"
#include "sema/nodes/shared/expr/array.h"
#include "sema/nodes/shared/expr/as.h"
#include "sema/nodes/shared/expr/binop.h"
#include "sema/nodes/shared/expr/call.h"
#include "sema/nodes/shared/expr/idx.h"
#include "sema/nodes/shared/expr/null.h"
#include "sema/nodes/shared/expr/ret_on_null.h"
#include "sema/nodes/shared/expr/structure.h"
#include "sema/nodes/shared/expr/unwrap.h"
#include "sema/nodes/shared/path.h"
#include "sema/value.h"
#include "sema/type/type.h"
#include "expr/unary.h"
#include "core/assert.h"

SemaValue *_sema_analyze_expr(SemaModule *sema, AstExpr *expr, SemaExprCtx ctx) {
    switch (expr->kind) {
        case AST_EXPR_LOCAL_PATH: return sema_resolve_path(sema, expr->local_path);
        case AST_EXPR_INNER_PATH:
            return sema_resolve_inner_path(sema, NOT_NULL(sema_analyze_expr(sema, expr->inner_path.inner, sema_expr_ctx_with(ctx, NULL))), expr->inner_path.path);
        case AST_EXPR_INTEGER:
            return sema_value_new_runtime(sema_value_runtime_new_const(sema_const_new_int(
                ctx.expectation && sema_type_is_int(ctx.expectation) ?
                    ctx.expectation : sema_arch_info_int(sema_module_arch_info(sema)),
                expr->integer
            )));
        case AST_EXPR_FP: 
            return sema_value_new_runtime(sema_value_runtime_new_const(sema_const_new_fp(
                ctx.expectation && sema_type_is_float(ctx.expectation) ?
                    ctx.expectation : sema_arch_info_float(sema_module_arch_info(sema)),
                expr->fp
            )));
        case AST_EXPR_CHAR:
            return sema_value_new_runtime(sema_value_runtime_new_const(sema_const_new_int(
                ctx.expectation && sema_type_is_int(ctx.expectation) ?
                    ctx.expectation : sema_type_primitive_u8(),
                expr->character
            )));
        case AST_EXPR_STR:
            return sema_value_new_runtime(sema_value_runtime_new_final(sema_type_new_slice(sema_type_primitive_u8())));
        case AST_EXPR_BOOLEAN:
            return sema_value_new_runtime(sema_value_runtime_new_const(sema_const_new_bool(sema_type_primitive_bool(), expr->boolean)));
        case AST_EXPR_UNARY:
            return sema_module_analyze_expr_unary(sema, &expr->unary);
        case AST_EXPR_ARRAY:
            return sema_module_analyze_expr_array(sema, expr->loc, &expr->array, ctx);
        case AST_EXPR_NULL:
            return sema_module_analyze_expr_null(sema, expr->loc, ctx);
        case AST_EXPR_SCOPE:
            return sema_analyze_expr(sema, expr->scope, ctx);
        case AST_EXPR_STRUCTURE:
            return sema_module_analyze_expr_structure(sema, &expr->structure);
        case AST_EXPR_IDX:
            return sema_module_analyze_expr_idx(sema, &expr->idx);
        case AST_EXPR_CALL:
            return sema_module_analyze_expr_call(sema, &expr->call);
        case AST_EXPR_RET_ON_NULL:
            return sema_module_analyze_expr_ret_on_null(sema, expr->ret_on_null);
        case AST_EXPR_UNWRAP:
            return sema_module_analyze_expr_unwrap(sema, &expr->unwrap);
        case AST_EXPR_AS:
            return sema_module_analyze_expr_as(sema, &expr->as, ctx);
        case AST_EXPR_BINOP:
            return sema_module_analyze_expr_binop(sema, &expr->binop);
        case AST_EXPR_ANON_FUNC: SEMA_ERROR(file_loc_new(), "anon funcs are NIY"); return NULL;
    }
    assert(0, "invalid ast expr");
}

SemaValue *sema_analyze_expr(SemaModule *sema, AstExpr *expr, SemaExprCtx ctx) {
    return expr->sema.value = _sema_analyze_expr(sema, expr, ctx);
}

SemaType *sema_analyze_runtime_expr(SemaModule *sema, AstExpr *expr, SemaExprCtx ctx) {
    SemaType *type = sema_value_is_runtime(NOT_NULL(sema_analyze_expr(sema, expr, ctx)));
    if (!type) {
        SEMA_ERROR(expr->loc, "expression must be runtime value");
    }
    return type;
}
