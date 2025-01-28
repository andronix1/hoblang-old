#include "../../parts.h"

void sema_stmt_var(SemaModule *sema, AstVar *var) {
	SemaType *var_type;
	if (var->typed) {
		if (!(var_type = sema_ast_type(sema, &var->type))) {
			return;
		}
		if (var->initializes) {
			SemaType *type = sema_ast_expr_type(sema, &var->expr, var_type);
			if (!type) {
				return;
			}
			if (!sema_types_equals(type, var_type)) {
				sema_err("cannot put {sema::type} in {sema::type} variable", type, var_type);
				return;
			}
		}
	} else {
		if (var->initializes) {
			if (!(var_type = sema_ast_expr_type(sema, &var->expr, NULL))) {
				return;
			}
			var->typed = true;
			var->type.sema = var->expr.sema_type; 
		} else {
			sema_err("variable type must be specified or initializer must present");
			return;
		}
	}
	var->decl = &sema_module_push_decl(sema, sema_scope_decl_new_value(var->name, var_type))->value_decl;
}
