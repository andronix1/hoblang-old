#include "ast/private/stmts/var.h"
#include "ast/private/expr.h"
#include "sema/module/private.h"
#include "sema/module/parts/expr.h"
#include "sema/module/parts/type.h"
#include "sema/value/api.h"
#include "sema/value/private.h"
#include "sema/type/api.h"
#include "sema/module/decls/api.h"

void sema_stmt_var(SemaModule *sema, bool public, AstVar *var) {
	SemaType *var_type;
	if (var->typed) {
		if (!(var_type = sema_ast_type(sema, &var->type))) {
			return;
		}
		if (var->initializes) {
			SemaType *type = sema_value_expr_type(sema, var->expr, sema_expr_ctx_default_of(var_type));
			if (!type) {
				return;
			}
			if (!sema_types_equals(type, var_type)) {
				SEMA_ERROR(var->expr->loc, "cannot put {sema::type} in {sema::type} variable", type, var_type);
				return;
			}
		}
	} else {
		if (var->initializes) {
			if (!(var_type = sema_value_expr_type(sema, var->expr, sema_expr_ctx_default_of(NULL)))) {
				return;
			}
			var->typed = true;
			var->type.sema = sema_value_typeof(var->expr->value); 
		} else {
			SEMA_ERROR(var->loc, "variable type must be specified or initializer must present");
			return;
		}
	}
	var->decl = sema_module_push_decl(sema, var->loc, public, sema_decl_new(var->name, sema_value_var(var_type)));
}
