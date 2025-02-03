#include "../parts.h"

SemaType *sema_ast_expr_array(SemaModule *sema, AstExpr **array, SemaType *expectation);
SemaType *sema_ast_expr_as(SemaModule *sema, AstExprAs *as);
SemaType *sema_ast_expr_binop(SemaModule *sema, AstExprBinop *binop, SemaType *expectation);
SemaType *sema_ast_call(SemaModule *sema, AstCall *call);
SemaType *sema_ast_expr_int(SemaModule *sema, uint64_t integer, SemaType *expectation);

SemaType *sema_ast_expr_type(SemaModule *sema, AstExpr *expr, SemaType *expectation) {
	switch (expr->type) {
		case AST_EXPR_GET_INNER_PATH: {
			SemaType *of_type = sema_ast_expr_type(sema, expr->get_inner.of, NULL);
			if (!of_type) {
				return NULL;
			}
			SemaResolvedPath resolved;
			if (!sema_resolve_inner_value_path(sema, of_type, &expr->get_inner.path, 0, &resolved)) {
				return NULL;
			}
			switch (resolved.kind) {
				case SEMA_RESOLVE_PATH_META_CONST:
				case SEMA_RESOLVE_PATH_META_VAR:
					return expr->sema_type = resolved.type;
				case SEMA_RESOLVE_PATH_META_TYPE:
					sema_err("inner path of expression must be value");
					return NULL;
			}
			assert(0, "resolved type was not cought");
			return NULL;
		}
		case AST_EXPR_GET_LOCAL_PATH:
			return expr->sema_type = sema_resolve_value_path(sema, &expr->get_local.path);
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
					return expr->sema_type = type;
				case AST_UNARY_BITNOT:
					if (type->type != SEMA_TYPE_PRIMITIVE) {
						sema_err("cannot apply unary bitnot to non-primitive types");
					}
					return expr->sema_type = type;
			}
			assert(0, "invalid unary type: {int}", expr->unary.type);
			break;
		}
		case AST_EXPR_REF:
			expr->sema_type = sema_ast_expr_type(sema, expr->ref_expr, NULL);
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
		case AST_EXPR_ARRAY: return expr->sema_type = sema_ast_expr_array(sema, expr->array, expectation);
		case AST_EXPR_INTEGER: return expr->sema_type = sema_ast_expr_int(sema, expr->integer, expectation);
		case AST_EXPR_BINOP: return expr->sema_type = sema_ast_expr_binop(sema, &expr->binop, expectation);
		case AST_EXPR_AS: return expr->sema_type = sema_ast_expr_as(sema, &expr->as);
		case AST_EXPR_CALL: return expr->sema_type = sema_ast_call(sema, &expr->call);
	}
	assert(0, "invalid expr type {int}", expr->type);
}
