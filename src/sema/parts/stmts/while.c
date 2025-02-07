#include "../../parts.h"

void sema_stmt_while_loop(SemaModule *sema, AstWhile *while_loop) {
	SemaType *type = sema_value_expr_type(sema, while_loop->expr, sema_expr_ctx_default_of(&primitives[PRIMITIVE_BOOL]));
	if (!type) {
		return;
	}
	if (!sema_types_equals(type, &primitives[PRIMITIVE_BOOL])) {
		sema_err("while loop condition expression must be boolean, not {sema::type}", type);
	}
	sema_ast_body(sema, while_loop->body);
}
