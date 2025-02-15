#include "exprs.h"
#include "core/vec.h"
#include "sema/module/parts/expr.h"
#include "sema/type/private.h"

SemaValue *sema_analyze_expr_call(SemaModule *sema, AstCall *call, SemaExprCtx ctx) { 
	SemaValue *value = sema_callable_expr_type(sema, call->callable, sema_expr_ctx_expect(ctx, NULL));
	SemaType *type = value->sema_type;
	if (!type) {
		return NULL;
	}
	if (type->type != SEMA_TYPE_FUNCTION) {	
		sema_err("`{ast::expr}` is not a function", call->callable);
		return NULL;
	}	
	size_t args_offset = value->type == SEMA_VALUE_EXT_FUNC_HANDLE;
	if (vec_len(call->args) != vec_len(type->func.args) - args_offset) {
		sema_err("expected {long} args to function {ast::expr} but {long} was passed", vec_len(type->func.args) - args_offset, call->callable, vec_len(call->args));
		return NULL;
	}
	for (size_t i = 0; i < vec_len(call->args); i++) {
        SemaType *arg_type = sema_value_expr_type(sema, call->args[i], sema_expr_ctx_expect(ctx, type->func.args[i + args_offset]));
		if (!arg_type) {
			return NULL;
		}
		if (!sema_types_equals(arg_type, type->func.args[i + args_offset])) {
			sema_err("expected {sema::type} arg type but {sema::type} was passed", type->func.args[i + args_offset], arg_type);
			return NULL;
		}
	}
	return sema_value_const(type->func.returning);
}
