#include "../../parts.h"

void sema_stmt_assign(SemaModule *sema, AstAssign *assign) {
	SemaType *assign_type = sema_var_expr_type(sema, assign->assign_expr, sema_expr_ctx_default());
	if (!assign_type) {
		return;
	}
	SemaType *expr_type = sema_value_expr_type(sema, assign->expr, sema_expr_ctx_default_of(assign_type));
	if (!expr_type) {
		return;
	}
	if (!sema_types_equals(assign_type, expr_type)) {
		sema_err("cannot assign expression with type `{sema::type}` to variable of type `{sema::type}`", expr_type, assign_type);
		return;
	}
}
