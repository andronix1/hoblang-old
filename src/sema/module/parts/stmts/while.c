#include "ast/private/stmts/while.h"
#include "sema/type/private.h"
#include "sema/module/private.h"
#include "sema/module/parts/expr.h"
#include "sema/module/parts/body.h"

void sema_stmt_while_loop(SemaModule *sema, AstWhile *while_loop) {
	SemaType *type = sema_value_expr_type(sema, while_loop->expr, sema_expr_ctx_default_of(sema_type_primitive_bool()));
	if (!type) {
		return;
	}
	if (!sema_types_equals(type, sema_type_primitive_bool())) {
		sema_err("while loop condition expression must be boolean, not {sema::type}", type);
	}
	sema_ast_body(sema, while_loop->body);
}
