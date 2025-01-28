#include "../../parts.h"

void sema_if_else_body(SemaModule *sema, AstIfBody *if_body) {
	SemaType *type = sema_ast_expr_type(sema, &if_body->expr, &primitives[PRIMITIVE_BOOL]);
	if (!type) {
		return;
	}
	if (!sema_types_equals(type, &primitives[PRIMITIVE_BOOL])) {
		sema_err("if condition expression must be boolean, not {sema::type}", type);
	}
	sema_ast_body(sema, if_body->body);
}

void sema_stmt_if_else(SemaModule *sema, AstIfElse *if_else) {
	sema_if_else_body(sema, &if_else->main);
	for (size_t i = 0; i < vec_len(if_else->else_ifs); i++) {
		sema_if_else_body(sema, &if_else->else_ifs[i]);
	}
	if (if_else->has_else) {
		sema_ast_body(sema, if_else->else_body);
	}
}
