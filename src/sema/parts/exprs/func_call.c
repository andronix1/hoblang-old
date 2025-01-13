SemaType *sema_ast_func_call(Sema *sema, AstFuncCall *func_call) {
	SemaType *type = sema_resolve_decl_type(sema, &func_call->name);
	if (!type) {
		sema_err("function `{slice}` is undefined", &func_call->name);
		return NULL;
	}
	if (type->type != SEMA_TYPE_FUNCTION) {	
		sema_err("`{slice}` is not a function", &func_call->name);
		return NULL;
	}	
	if (vec_len(func_call->args) != vec_len(type->func.args)) {
		sema_err("expected {long} args to function {slice} but {long} was passed", vec_len(type->func.args), &func_call->name, vec_len(func_call->args));
		return NULL;
	}
	for (size_t i = 0; i < vec_len(func_call->args); i++) {
		SemaType *arg_type = sema_ast_expr_type(sema, &func_call->args[i], type->func.args[i].type.sema);
		if (!arg_type) {
			return NULL;
		}
		if (!sema_types_equals(arg_type, type->func.args[i].type.sema)) {
			sema_err("expected {sema::type} arg type but {sema::type} was passed", type->func.args[i].type.sema, arg_type);
			return NULL;
		}
	}
	return type->func.returning;
}
