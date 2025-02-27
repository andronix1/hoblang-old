#include "exprs.h"
#include "core/vec.h"
#include "sema/module/parts/expr.h"
#include "sema/module/private.h"
#include "sema/type/private.h"

SemaValue *sema_analyze_expr_call(SemaModule *sema, AstCall *call, SemaExprCtx ctx) { 
	SemaValue *value = sema_callable_expr_type(sema, call->callable, sema_expr_ctx_expect(ctx, NULL));
    if (!value) {
        return NULL;
    }
	SemaType *type = value->sema_type;
	if (!type) {
		return NULL;
	}
	if (type->type != SEMA_TYPE_FUNCTION) {	
		SEMA_ERROR(ctx.loc, "{sema::type} is not a function, so it cannot be called", type);
		return NULL;
	}	
	size_t args_offset = value->type == SEMA_VALUE_EXT_FUNC_HANDLE;
	if (vec_len(call->args) != vec_len(type->func.args) - args_offset) {
		SEMA_ERROR(ctx.loc, "expected {long} args to function {ast::expr} but {long} was passed", vec_len(type->func.args) - args_offset, call->callable, vec_len(call->args));
		return NULL;
	}
	for (size_t i = 0; i < vec_len(call->args); i++) {
        SemaType *arg_type = sema_value_expr_type(sema, call->args[i], sema_expr_ctx_expect(ctx, type->func.args[i + args_offset]));
		if (!arg_type) {
			return NULL;
		}
		if (!sema_types_equals(arg_type, type->func.args[i + args_offset])) {
			SEMA_ERROR(call->args[i]->loc, "arg №{long} expected to has type {sema::type} but expression of type {sema::type} was passed", i + 1, type->func.args[i + args_offset], arg_type);
			return NULL;
		}
	}
	return sema_value_const(type->func.returning);
}
