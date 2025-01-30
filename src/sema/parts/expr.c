#include "../parts.h"

SemaType *sema_ast_expr_array(SemaModule *sema, AstExpr *array, SemaType *expectation);
SemaType *sema_ast_expr_as(SemaModule *sema, AstExprAs *as);
SemaType *sema_ast_expr_binop(SemaModule *sema, AstExprBinop *binop, SemaType *expectation);
SemaType *sema_ast_func_call(SemaModule *sema, AstFuncCall *func_call);
SemaType *sema_ast_expr_int(SemaModule *sema __attribute__((unused)), uint64_t integer __attribute__((unused)), SemaType *expectation);

SemaType *sema_ast_expr_type(SemaModule *sema, AstExpr *expr, SemaType *expectation) {
	switch (expr->type) {
		case AST_EXPR_VALUE: return expr->sema_type = sema_ast_value(sema, &expr->value);
		case AST_EXPR_UNARY: {
			SemaType *type = sema_ast_expr_type(sema, expr->unary.expr, expectation);
			if (!type) {
				return NULL;
			}
			switch (expr->unary.type) {
				case AST_UNARY_MINUS:
					if (type->type != SEMA_TYPE_PRIMITIVE) {
						sema_err("cannot apply unary minus to non-primitive types");
					}
					return type;
				case AST_UNARY_BITNOT:
					if (type->type != SEMA_TYPE_PRIMITIVE) {
						sema_err("cannot apply unary bitnot to non-primitive types");
					}
					return type;
			}
			assert(0, "invalid unary type: {int}", expr->unary.type);
			break;
		}
		case AST_EXPR_REF:
			expr->sema_type = sema_ast_value(sema, &expr->value);
			if (!expr->sema_type) {
				return NULL;
			}
			return expr->sema_type = sema_type_new_pointer(expr->sema_type);
		case AST_EXPR_NOT:
			expr->sema_type = sema_ast_expr_type(sema, expr->not_expr, &primitives[PRIMITIVE_BOOL]);
			if (!sema_types_equals(expr->sema_type, &primitives[PRIMITIVE_BOOL])) {
				sema_err("not operator can be only be applied to booleans, not {sema::type}", expr->sema_type);
			}
			return expr->sema_type = &primitives[PRIMITIVE_BOOL];
		case AST_EXPR_STR: return expr->sema_type = sema_type_new_slice(&primitives[PRIMITIVE_U8]);
		case AST_EXPR_CHAR: return expr->sema_type = &primitives[PRIMITIVE_U8];
		case AST_EXPR_BOOL: return expr->sema_type = &primitives[PRIMITIVE_BOOL];
		// case AST_EXPR_ARRAY: return expr->sema_type = sema_ast_expr_array(sema, expr->array, expectation);
		case AST_EXPR_INTEGER: return expr->sema_type = sema_ast_expr_int(sema, expr->integer, expectation);
		case AST_EXPR_BINOP: return expr->sema_type = sema_ast_expr_binop(sema, &expr->binop, expectation);
		case AST_EXPR_AS: return expr->sema_type = sema_ast_expr_as(sema, &expr->as);
		case AST_EXPR_FUNCALL: return expr->sema_type = sema_ast_func_call(sema, &expr->func_call);
	}
	assert(0, "invalid expr type {int}", expr->type);
}
