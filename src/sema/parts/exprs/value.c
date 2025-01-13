SemaType *sema_ast_expr_value(Sema *sema, Slice *value) {
	SemaType *decl_type = sema_resolve_decl_type(sema, value);
	if (!decl_type) {
		sema_err("{slice} is undeclared", value);
		return NULL;
	}
	return decl_type;
}
