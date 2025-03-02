#include "ast/private/expr/binop.h"
#include "exprs.h"
#include "sema/const/const.h"
#include "sema/const/api.h"
#include "sema/module/private.h"
#include "sema/type/private.h"

SemaValue *sema_expr_binop_eval(AstBinopType type, SemaConst *left, SemaConst *right) {
    #define BO(_op) \
        return sema_value_const(sema_const_bool(left->boolean _op right->boolean));
    #define IO(_op) \
        return sema_value_const(sema_const_int(left->sema_type, left->integer _op right->integer));
    #define IFO(_op) \
        do { \
            if (sema_type_is_int(left->sema_type)) { \
                IO(_op) \
            } else if (sema_type_is_float(left->sema_type)) { \
                return sema_value_const(sema_const_float(left->sema_type, left->fp _op right->fp)); \
            } else { \
                assert(0, "only floats or integers allowed"); \
            } \
        } while(0)

    switch (type) {
        case AST_BINOP_ADD: IFO(+);
        case AST_BINOP_MOD: IO(%);
        case AST_BINOP_SUB: IFO(-);
        case AST_BINOP_MUL: IFO(*);
        case AST_BINOP_DIV: IFO(/);
        case AST_BINOP_BITAND: IO(&);
        case AST_BINOP_BITOR: IO(|);
        case AST_BINOP_SHR: IO(>>);
        case AST_BINOP_SHL: IO(<<);
        case AST_BINOP_XOR: IO(^);

        case AST_BINOP_EQ: IFO(==);
        case AST_BINOP_NEQ: IFO(!=);
        case AST_BINOP_GT: IFO(>=);
        case AST_BINOP_GE: IFO(<=);
        case AST_BINOP_LT: IFO(>);
        case AST_BINOP_LE: IFO(<);

        case AST_BINOP_AND: BO(&&);
        case AST_BINOP_OR: BO(||);
    }
    assert(0, "invalid binop type");
}

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
    if (
            binop->left->value->type == SEMA_VALUE_CONST &&
            binop->right->value->type == SEMA_VALUE_CONST
    ) {
        return sema_expr_binop_eval(binop->type, &binop->left->value->constant, &binop->right->value->constant);
    }
	return sema_value_final(ltype);
}
