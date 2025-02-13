#include "sema/module/private.h"
#include "sema/module/parts/expr.h"
#include "sema/type/private.h"
#include "sema/value/private.h"
#include "ast/private/expr.h"
#include "core/assert.h"

SemaValue *sema_analyze_expr_array(SemaModule *sema, AstExpr **array, SemaExprCtx ctx);
SemaValue *sema_analyze_expr_as(SemaModule *sema, AstExprAs *as, SemaExprCtx ctx);
SemaValue *sema_analyze_expr_binop(SemaModule *sema, AstExprBinop *binop, SemaExprCtx ctx);
SemaValue *sema_analyze_expr_call(SemaModule *sema, AstCall *call, SemaExprCtx ctx);
SemaValue *sema_analyze_expr_int(SemaModule *sema, uint64_t integer, SemaExprCtx ctx);
SemaValue *sema_analyze_expr_ref(SemaModule *sema, AstExpr *ref_expr, SemaExprCtx ctx);
SemaValue *sema_analyze_expr_not(SemaModule *sema, AstExpr *not_expr, SemaExprCtx ctx);
SemaValue *sema_analyze_expr_unary(SemaModule *sema, AstExprUnary *unary, SemaExprCtx ctx);
SemaValue *sema_analyze_expr_get_inner(SemaModule *sema, AstExprGetInner *get_inner, SemaExprCtx ctx);
SemaValue *sema_analyze_expr_get_local(SemaModule *sema, AstExprGetLocal *get_local, SemaExprCtx ctx);

SemaValue *sema_expr(SemaModule *sema, AstExpr *expr, SemaExprCtx ctx) {
    switch (expr->type) {
		case AST_EXPR_UNARY: return expr->value = sema_analyze_expr_unary(sema, &expr->unary, ctx);
		case AST_EXPR_GET_INNER_PATH: return expr->value = sema_analyze_expr_get_inner(sema, &expr->get_inner, ctx);
		case AST_EXPR_GET_LOCAL_PATH: return expr->value = sema_analyze_expr_get_local(sema, &expr->get_local, ctx);
		case AST_EXPR_REF: return expr->value = sema_analyze_expr_ref(sema, expr->ref_expr, ctx);
		case AST_EXPR_NOT: return expr->value = sema_analyze_expr_not(sema, expr->not_expr, ctx);
		case AST_EXPR_STR: return expr->value = sema_value_const(sema_type_new_slice(sema_type_primitive_u8()));
		case AST_EXPR_CHAR: return expr->value = sema_value_const(sema_type_primitive_u8());
		case AST_EXPR_BOOL: return expr->value = sema_value_const(sema_type_primitive_bool());
		case AST_EXPR_ARRAY: return expr->value = sema_analyze_expr_array(sema, expr->array, ctx);
		case AST_EXPR_BINOP: return expr->value = sema_analyze_expr_binop(sema, &expr->binop, ctx);
		case AST_EXPR_AS: return expr->value = sema_analyze_expr_as(sema, &expr->as, ctx);
		case AST_EXPR_INTEGER: return expr->value = sema_analyze_expr_int(sema, expr->integer, ctx);
        case AST_EXPR_CALL: return expr->value = sema_analyze_expr_call(sema, &expr->call, ctx);
    }
	assert(0, "invalid ast expr type: {int}", expr->type);
}

SemaType *sema_const_expr_type(SemaModule *sema, AstExpr *expr, SemaExprCtx ctx) {
    if (!sema_expr(sema, expr, ctx)) {
        return NULL;
    }
    if (expr->value->type != SEMA_VALUE_CONST) {
        sema_err("`{ast::expr}` is not a const", expr);
    }
    return expr->value->sema_type;
}

SemaType *sema_var_expr_type(SemaModule *sema, AstExpr *expr, SemaExprCtx ctx) {
    if (!sema_expr(sema, expr, ctx)) {
        return NULL;
    }
    if (expr->value->type != SEMA_VALUE_VAR) {
        sema_err("`{ast::expr}` is not a variable", expr);
    }
    return expr->value->sema_type;
}

SemaType *sema_value_expr_type(SemaModule *sema, AstExpr *expr, SemaExprCtx ctx) {
    if (!sema_expr(sema, expr, ctx)) {
        return NULL;
    }
    if (expr->value->type != SEMA_VALUE_CONST && expr->value->type != SEMA_VALUE_VAR) {
        sema_err("`{ast::expr}` is not a value", expr);
    }
    return expr->value->sema_type;
}
