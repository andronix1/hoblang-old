#include <stdbool.h>
#include "ast/private/stmts/return.h"
#include "ast/private/expr.h"
#include "sema/module/private.h"
#include "sema/type/private.h"
#include "sema/module/parts/expr.h"

void sema_stmt_return(SemaModule *sema, AstReturn *ret) {
    SemaType *returns = sema_module_returns(sema);
	if (ret->has_value) {
		SemaType *type = sema_value_expr_type(sema, ret->expr, sema_expr_ctx_default_of(returns));
		if (!type) {
			return;
		}
		if (!sema_types_equals(returns, type)) {
			SEMA_ERROR(ret->expr->loc, "function must return {sema::type} but trying to return expression of type {sema::type}", returns, type);
			return;
		}
	} else {
		if (!sema_types_equals(returns, sema_type_primitive_void())) {
			SEMA_ERROR(ret->loc, "expected to return `{sema::type}` but got nothing", returns);
			return;
		}
	}
    ret->defers = sema_module_defers_up_to(sema, NULL);
}
