#include "ast/private/stmts/while.h"
#include "sema/module/loop/api.h"
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
    while_loop->loop = while_loop->is_named ?
        sema_loop_new_named(while_loop->name) :
        sema_loop_new();
    sema_module_push_loop(sema, while_loop->loop);
	sema_ast_body(sema, while_loop->body);
    sema_module_pop_loop(sema);
}
