#include "../parts.h"

bool sema_ast_func_call(Sema *sema, SemaType *output_type, AstFuncCall *func_call) {
	SemaType *type = sema_resolve_decl_type(sema, &func_call->name);
	if (!type) {
		sema_err("function `{slice}` is undefined", &func_call->name);
		return false;
	}
	if (type->type != SEMA_TYPE_FUNCTION) {	
		sema_err("`{slice}` is not a function", &func_call->name);
		return false;
	}	
	if (vec_len(func_call->args) != vec_len(type->func.args)) {
		sema_err("expected {long} args to function {slice} but {long} was passed", vec_len(type->func.args), &func_call->name, vec_len(func_call->args));
		return false;
	}
	for (size_t i = 0; i < vec_len(func_call->args); i++) {
		SemaType arg_type;
		if (!sema_ast_expr_type(sema, &arg_type, &func_call->args[i], type->func.args[i].type.sema)) {
			return false;
		}
		if (!sema_types_equals(&arg_type, type->func.args[i].type.sema)) {
			sema_err("expected {sema::type} arg type but {sema::type} was passed", type->func.args[i].type.sema, &arg_type);
			return false;
		}
	}
	if (output_type) {
		memcpy(output_type, type->func.returning, sizeof(SemaType));
	}
	return true;
}

void sema_expr_try_cast(AstExpr *expr, SemaType *to) {
	int primitive_levels[] = {
		[PRIMITIVE_VOID] = 100,
		[PRIMITIVE_BOOL] = 50,
		[PRIMITIVE_U8] = 40,
		[PRIMITIVE_I8] = 40,
		[PRIMITIVE_U16] = 30,
		[PRIMITIVE_I16] = 30,
		[PRIMITIVE_U32] = 20,
		[PRIMITIVE_I32] = 20,
		[PRIMITIVE_U64] = 10,
		[PRIMITIVE_I64] = 10,
	};
	if (!to || expr->sema_type->type != SEMA_TYPE_PRIMITIVE || to->type != SEMA_TYPE_PRIMITIVE) {
		return;
	}
	if (primitive_levels[expr->sema_type->primitive] >= primitive_levels[to->primitive]) {
		expr->sema_type = malloc(sizeof(SemaType));
		memcpy(expr->sema_type, to, sizeof(SemaType));
	}
}

bool sema_ast_expr_type(Sema *sema, SemaType *type, AstExpr *expr, SemaType *expectation) {
	switch (expr->type) {
		case AST_EXPR_VALUE: {
			SemaType *decl_type = sema_resolve_decl_type(sema, &expr->value);
			if (!decl_type) {
				sema_err("{slice} is undeclared", &expr->value);
				return false;
			}
			expr->sema_type = decl_type;
			*type = *expr->sema_type;
			return true;
		}
		case AST_EXPR_CHAR: {
			expr->integer = expr->character;
			expr->sema_type = &primitives[PRIMITIVE_U8];
			sema_expr_try_cast(expr, expectation);
			*type = *expr->sema_type;
			return true;
		}
		case AST_EXPR_BOOL: {
			expr->integer = expr->boolean;
			expr->sema_type = &primitives[PRIMITIVE_BOOL];
			sema_expr_try_cast(expr, expectation);
			*type = *expr->sema_type;
			return true;
		}
		case AST_EXPR_INTEGER: {
			Primitive min_primitive = PRIMITIVE_U8;
			if (expr->integer > 0xFFFF) min_primitive = PRIMITIVE_U16;
			if (expr->integer > 0xFFFFFFFF) min_primitive = PRIMITIVE_U32;
			if (expr->integer > 0xFFFFFFFFFFFFFFFF) min_primitive = PRIMITIVE_U64;
			expr->sema_type = &primitives[min_primitive];
			sema_expr_try_cast(expr, expectation);
			*type = *expr->sema_type;
			return true;
		}
		case AST_EXPR_BINOP: {
			bool binary_binop = (
					expr->binop.type == AST_BINOP_EQ ||
					expr->binop.type == AST_BINOP_NEQ ||
					expr->binop.type == AST_BINOP_GT ||
					expr->binop.type == AST_BINOP_GE ||
					expr->binop.type == AST_BINOP_LT || 
					expr->binop.type == AST_BINOP_LE
				);
			SemaType *expect = binary_binop ? NULL : expectation;
			if (!sema_ast_expr_type(sema, expr->sema_type = malloc(sizeof(SemaType)), expr->binop.left, expect)) {
				return false;
			}
			if (expr->sema_type->type != SEMA_TYPE_PRIMITIVE || expr->sema_type->primitive == PRIMITIVE_VOID) {
				sema_err("cannot use binop for type {sema::type}", expr->sema_type);
				return false;
			}
			if (binary_binop) {
				expect = expr->sema_type;
			}
			SemaType right_type;
			if (!sema_ast_expr_type(sema, &right_type, expr->binop.right, expect)) {
				return false;
			}
			if (!sema_types_equals(expr->sema_type, &right_type)) {
				sema_err("cannot use binop {ast::binop} for types {sema::type} and {sema::type}", expr->binop.type, expr->sema_type, &right_type);
				return false;
			}
			if (binary_binop) {
				expr->sema_type = &primitives[PRIMITIVE_BOOL];
			}
			*type = *expr->sema_type;
			return true;
		}
		case AST_EXPR_AS: {
			if (!sema_ast_type(sema, expr->sema_type = malloc(sizeof(SemaType)), &expr->as.type)) {
				return false;
			}
			SemaType expr_type;
			if (!sema_ast_expr_type(sema, &expr_type, expr->as.expr, NULL)) {
				return false;
			}
			if (expr_type.type != SEMA_TYPE_PRIMITIVE || expr->sema_type->type != SEMA_TYPE_PRIMITIVE || expr_type.primitive == PRIMITIVE_VOID || expr->sema_type->primitive == PRIMITIVE_VOID) {
				sema_err("only primitives can be casted");
				return false;
			}
			*type = *expr->sema_type;
			return true;
		}
		case AST_EXPR_FUNCALL: {
			if (!sema_ast_func_call(sema, expr->sema_type = malloc(sizeof(SemaType)), &expr->func_call)) {
				return false;
			}
			*type = *expr->sema_type;
			return true;
		}
	}
	assert(0, "invalid expr type %d", expr->type);
}
