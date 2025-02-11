#include "parts.h"
#include "expr.h"
#include "ast/body.h"
#include "core/vec.h"

void sema_stmt_assign(SemaModule *sema, AstAssign *assign);
void sema_stmt_defer(SemaModule *sema, AstDefer *defer);
void sema_stmt_if_else(SemaModule *sema, AstIfElse *if_else);
void sema_stmt_return(SemaModule *sema, AstReturn *ret);
void sema_stmt_var(SemaModule *sema, AstVar *var);
void sema_stmt_while_loop(SemaModule *sema, AstWhile *while_loop);
void sema_stmt_inline_asm(SemaModule *sema, AstInlineAsm *inline_asm);

void sema_ast_body(SemaModule *sema, AstBody *body) {
	for (size_t i = 0; i < vec_len(body->stmts); i++) {
		AstStmt *stmt = &body->stmts[i];
		switch (stmt->type) {
			case AST_STMT_VAR: sema_stmt_var(sema, &stmt->var); break;
			case AST_STMT_RETURN: sema_stmt_return(sema, &stmt->ret); break;
			case AST_STMT_WHILE: sema_stmt_while_loop(sema, &stmt->while_loop); break;
			case AST_STMT_IF: sema_stmt_if_else(sema, &stmt->if_else); break;
			case AST_STMT_ASSIGN: sema_stmt_assign(sema, &stmt->assign); break;
			case AST_STMT_DEFER: sema_stmt_defer(sema, &stmt->defer); break;
			case AST_STMT_INLINE_ASM: sema_stmt_inline_asm(sema, &stmt->inline_asm); break;
			case AST_STMT_EXPR: sema_value_expr_type(sema, stmt->expr, sema_expr_ctx_default()); break;
		}
	}
	body->defers = sema_module_resolve_defers(sema);
} 
