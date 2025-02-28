#include "exprs.h"
#include "sema/module/private.h"
#include "sema/type/private.h"

SemaValue *sema_analyze_expr_binop(SemaModule *sema, AstExprBinop *binop, SemaExprCtx ctx) { 
    ctx.loc = binop->loc;
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
	SemaType *expect = binary_binop ? 
		(bool_binops ? sema_type_primitive_bool() : NULL) :
		ctx.expectation;
	SemaType *ltype = sema_value_expr_type(sema, binop->left, sema_expr_ctx_expect(ctx, expect));
	if (!ltype) {
		return NULL;
	}
	if (!binary_binop && (ltype->type != SEMA_TYPE_PRIMITIVE || (
        ltype->primitive.type != SEMA_PRIMITIVE_FLOAT &&
        ltype->primitive.type != SEMA_PRIMITIVE_INT
    ))) {
		SEMA_ERROR(ctx.loc, "binop {ast::binop} can be used for integers and floats only, not {sema::type}", binop->type, ltype);
		return NULL;
	}
	if (!bool_binops) {
		expect = ltype;
	}
	SemaType *right_type = sema_value_expr_type(sema, binop->right, sema_expr_ctx_expect(ctx, expect));
	if (!right_type) {
		return NULL;
	}
	if (bool_binops) {
		if (!sema_types_equals(ltype, sema_type_primitive_bool()) || !sema_types_equals(right_type, sema_type_primitive_bool())) {
			SEMA_ERROR(ctx.loc, "binop {ast::binop} can only operate booleans");
			return NULL;
		}
	}
	if (!sema_types_equals(ltype, right_type)) {
		SEMA_ERROR(ctx.loc, "binop {ast::binop} can operate identical typed, but {sema::type} != {sema::type}", binop->type, ltype, right_type);
		return NULL;
	}
	if (binary_binop) {
		ltype = sema_type_primitive_bool();
	}
    // TODO: binop between constants
	return sema_value_final(ltype);
}
