#include "../parts.h"
#include "stmts/assign.c"
#include "stmts/if_else.c"
#include "stmts/return.c"
#include "stmts/var.c"
#include "stmts/while.c"

void sema_ast_body(Sema *sema, AstBody *body) {
	for (size_t i = 0; i < vec_len(body->stmts); i++) {
		AstStmt *stmt = &body->stmts[i];
		switch (stmt->type) {
			case AST_STMT_VAR: sema_stmt_var(sema, &stmt->var); break;
			case AST_STMT_RETURN: sema_stmt_return(sema, &stmt->ret); break;
			case AST_STMT_WHILE: sema_stmt_while_loop(sema, &stmt->while_loop); break;
			case AST_STMT_IF: sema_stmt_if_else(sema, &stmt->if_else); break;
			case AST_STMT_ASSIGN: sema_stmt_assign(sema, &stmt->assign); break;
			case AST_STMT_FUNC_CALL: sema_ast_func_call(sema, &stmt->func_call); break;
		}
	}
} 
