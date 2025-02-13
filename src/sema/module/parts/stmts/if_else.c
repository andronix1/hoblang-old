#include "ast/private/stmts/if_else.h"
#include "core/vec.h"
#include "sema/module/private.h"
#include "sema/type/private.h"
#include "sema/module/parts/body.h"
#include "sema/module/parts/expr.h"

void sema_if_else_body(SemaModule *sema, AstIfBody *if_body) {
	SemaType *type = sema_value_expr_type(sema, if_body->expr, sema_expr_ctx_default_of(sema_type_primitive_bool()));
	if (!type) {
		return;
	}
	if (!sema_types_equals(type, sema_type_primitive_bool())) {
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
