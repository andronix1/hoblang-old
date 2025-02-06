#include "../../parts.h"

bool sema_analyze_expr_binop(SemaModule *sema, AstExprBinop *binop, SemaType *expectation, SemaValue *value) { 
	bool bool_binops = (
			binop->type == AST_BINOP_OR ||
			binop->type == AST_BINOP_AND
		);
	bool binary_binop = (
			binop->type == AST_BINOP_EQ ||
			binop->type == AST_BINOP_NEQ ||
			binop->type == AST_BINOP_GT ||
			binop->type == AST_BINOP_GE ||
			binop->type == AST_BINOP_LT || 
			binop->type == AST_BINOP_LE ||
			bool_binops
		);
	SemaType *expect = binary_binop ? (bool_binops ? &primitives[PRIMITIVE_BOOL] : NULL) : expectation;
	SemaType *ltype = sema_value_expr_type(sema, binop->left, expect);
	if (!ltype) {
		return false;
	}
	if ((ltype->type != SEMA_TYPE_PRIMITIVE && ltype->type != SEMA_TYPE_POINTER) || ltype->primitive == PRIMITIVE_VOID) {
		sema_err("cannot use binop for type {sema::type}", ltype);
		return false;
	}
	if (binary_binop) {
		expect = ltype;
	}
	SemaType *right_type = sema_value_expr_type(sema, binop->right, expect);
	if (!right_type) {
		return false;
	}
	if (bool_binops) {
		if (!sema_types_equals(ltype, &primitives[PRIMITIVE_BOOL]) || !sema_types_equals(right_type, &primitives[PRIMITIVE_BOOL])) {
			sema_err("boolean binops can only operate booleans");
			return false;
		}
	}
	if (!sema_types_equals(ltype, right_type)) {
		sema_err("cannot use binop {ast::binop} for types {sema::type} and {sema::type}", binop->type, ltype, &right_type);
		return false;
	}
	if (binary_binop) {
		ltype = &primitives[PRIMITIVE_BOOL];
	}
	return sema_value_const(value, ltype);
}
