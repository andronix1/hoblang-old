#include "ast/private/stmts/defer.h"
#include "../parts.h"
#include "../body.h"

void sema_stmt_defer(SemaModule *sema, AstDefer *defer) {
	sema_ast_body(sema, defer->body);
	sema_module_push_defer(sema, defer);
}
