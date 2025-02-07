#include "../../parts.h"

bool sema_analyze_expr_call(SemaModule *sema, AstCall *call, SemaExprCtx ctx) { 
	SemaType *type = sema_value_expr_type(sema, call->callable, sema_expr_ctx_expect(ctx, NULL));
	if (!type) {
		return false;
	}
	if (type->type != SEMA_TYPE_FUNCTION) {	
		sema_err("`{ast::expr}` is not a function", call->callable);
		return false;
	}	
	if (vec_len(call->args) != vec_len(type->func.args)) {
		sema_err("expected {long} args to function {ast::expr} but {long} was passed", vec_len(type->func.args), &call->callable, vec_len(call->args));
		return false;
	}
	for (size_t i = 0; i < vec_len(call->args); i++) {
        SemaType *arg_type = sema_value_expr_type(sema, call->args[i], sema_expr_ctx_expect(ctx, type->func.args[i]));
		if (!arg_type) {
			return false;
		}
		if (!sema_types_equals(arg_type, type->func.args[i])) {
			sema_err("expected {sema::type} arg type but {sema::type} was passed", type->func.args[i], arg_type);
			return false;
		}
	}
	return sema_value_const(ctx.value, type->func.returning);
}
