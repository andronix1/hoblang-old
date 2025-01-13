SemaType *sema_ast_expr_as(Sema *sema, AstExprAs *as) {
	SemaType *as_type = sema_ast_type(sema, &as->type);
	if (!as_type) {
		return NULL;
	}
	SemaType *expr_type = sema_ast_expr_type(sema, as->expr, NULL);
	if (!expr_type) {
		return NULL;
	}
	static bool shown_as_warn = false;
	if (!shown_as_warn) {
		hob_log(LOGW, "as expression is unsafe now so it can cause compiler bug(and segfaults :P)");
		shown_as_warn = true;
	}
	return as_type;
}
