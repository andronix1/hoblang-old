void sema_stmt_assign(Sema *sema, AstAssign *assign) {
	SemaType *decl_type = sema_resolve_decl_type(sema, &assign->name);
	if (!decl_type) {
		sema_err("{slice} is undefined", &assign->name);
		return;
	}
	SemaType *expr_type = sema_ast_expr_type(sema, &assign->expr, decl_type);
	if (!expr_type) {
		return;
	}
	if (!sema_types_equals(decl_type, expr_type)) {
		sema_err("cannot assign expression with type `{sema::type}` to variable of type `{sema::type}`", expr_type, decl_type);
		return;
	}
}
