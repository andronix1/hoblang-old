void sema_stmt_var(Sema *sema, AstVar *var) {
	SemaRecord decl = {
		.name = var->name,
	};
	if (var->typed) {
		if (!(decl.type = sema_ast_type(sema, &var->type))) {
			return;
		}
		if (var->initializes) {
			SemaType *type = sema_ast_expr_type(sema, &var->expr, decl.type);
			if (!type) {
				return;
			}
			if (!sema_types_equals(type, decl.type)) {
				sema_err("cannot put {sema::type} in {sema::type} variable", type, &decl.type);
				return;
			}
		}
	} else {
		if (var->initializes) {
			if (!(decl.type = sema_ast_expr_type(sema, &var->expr, NULL))) {
				return;
			}
			var->typed = true;
			var->type.sema = var->expr.sema_type; 
		} else {
			sema_err("variable type must be specified or initializer must present");
			return;
		}
	}
	sema_push_decl(sema, &decl);
}
