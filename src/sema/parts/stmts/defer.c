#include "../../parts.h"

void sema_stmt_defer(SemaModule *sema, AstDefer *defer) {
	sema_ast_body(sema, defer->body);
	sema_push_defer(sema, defer);
}
