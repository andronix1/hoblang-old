#include "../parts.h"
#include "exprs/func_call.c"
#include "exprs/as.c"
#include "exprs/binop.c"
#include "exprs/int.c"
#include "exprs/array.c"
#include "exprs/idx.c"
#include "exprs/value.c"

SemaType *sema_ast_expr_type(Sema *sema, AstExpr *expr, SemaType *expectation) {
	switch (expr->type) {
		case AST_EXPR_VALUE: return expr->sema_type = sema_ast_expr_value(sema, &expr->value);
		case AST_EXPR_STR: return expr->sema_type = sema_type_new_pointer(&primitives[PRIMITIVE_U8]);
		case AST_EXPR_CHAR: return expr->sema_type = &primitives[PRIMITIVE_U8];
		case AST_EXPR_BOOL: return expr->sema_type = &primitives[PRIMITIVE_BOOL];
		case AST_EXPR_IDX: return expr->sema_type = sema_ast_expr_idx(sema, &expr->idx, expectation);
		case AST_EXPR_ARRAY: return expr->sema_type = sema_ast_expr_array(sema, expr->array, expectation);
		case AST_EXPR_INTEGER: return expr->sema_type = sema_ast_expr_int(sema, expr->integer, expectation);
		case AST_EXPR_BINOP: return expr->sema_type = sema_ast_expr_binop(sema, &expr->binop, expectation);
		case AST_EXPR_AS: return expr->sema_type = sema_ast_expr_as(sema, &expr->as);
		case AST_EXPR_FUNCALL: return expr->sema_type = sema_ast_func_call(sema, &expr->func_call);
	}
	assert(0, "invalid expr type {int}", expr->type);
}
