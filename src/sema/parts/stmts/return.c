#include "../../parts.h"
#include "sema/type/private.h"

void sema_stmt_return(SemaModule *sema, AstReturn *ret) {
    SemaType *returns = sema_module_returns(sema);
	if (ret->has_value) {
		SemaType *type = sema_value_expr_type(sema, ret->expr, sema_expr_ctx_default_of(returns));
		if (!type) {
			return;
		}
		if (!sema_types_equals(returns, type)) {
			sema_err("expected to return `{sema::type}` got `{sema::type}`", returns, type);
			return;
		}
	} else {
		if (!sema_types_equals(returns, &primitives[PRIMITIVE_VOID])) {
			sema_err("expected to return `{sema::type}` but got nothing");
			return;
		}
	}
}
