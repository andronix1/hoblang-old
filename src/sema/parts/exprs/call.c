#include "../../parts.h"

SemaType *sema_ast_call(SemaModule *sema, AstCall *call) {
	SemaType *type = sema_ast_expr_type(sema, call->callable, NULL);
	if (!type) {
		return NULL;
	}
	if (type->type != SEMA_TYPE_FUNCTION) {	
		sema_err("`{ast::expr}` is not a function", call->callable);
		return NULL;
	}	
	if (vec_len(call->args) != vec_len(type->func.args)) {
		sema_err("expected {long} args to function {ast::expr} but {long} was passed", vec_len(type->func.args), &call->callable, vec_len(call->args));
		return NULL;
	}
	for (size_t i = 0; i < vec_len(call->args); i++) {
		SemaType *arg_type = sema_ast_expr_type(sema, call->args[i], type->func.args[i]);
		if (!arg_type) {
			return NULL;
		}
		if (!sema_types_equals(arg_type, type->func.args[i])) {
			sema_err("expected {sema::type} arg type but {sema::type} was passed", type->func.args[i], arg_type);
			return NULL;
		}
	}
	return type->func.returning;
}
