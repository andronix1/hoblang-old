#include "ast/api/defer.h"
#include "ast/private/expr.h"
#include "sema/module/private.h"
#include "sema/module/parts/expr.h"
#include "sema/module/parts/val_decl.h"
#include "ast/private/body.h"
#include "core/vec.h"

void sema_stmt_loop_control(SemaModule *sema, AstStmtLoopControl *loop_control);
void sema_stmt_assign(SemaModule *sema, AstAssign *assign);
void sema_stmt_defer(SemaModule *sema, AstDefer *defer);
void sema_stmt_if_else(SemaModule *sema, AstIfElse *if_else);
void sema_stmt_return(SemaModule *sema, AstReturn *ret);
void sema_stmt_while_loop(SemaModule *sema, AstWhile *while_loop);
void sema_stmt_inline_asm(SemaModule *sema, AstInlineAsm *inline_asm);

void sema_ast_body(SemaModule *sema, AstBody *body) {
    body->defers = vec_new(AstDefer*);
	sema_module_push_body_scope(sema, body);
	for (size_t i = 0; i < vec_len(body->stmts); i++) {
		AstStmt *stmt = &body->stmts[i];
		switch (stmt->type) {
			case AST_STMT_VAL_DECL: {
                SemaDecl *decl = sema_val_decl(sema, &stmt->val_decl, false);
                if (!decl) {
                    break;
                }
                sema_module_push_scope_decl(sema, stmt->val_decl.loc, decl);
                break;
            }
			case AST_STMT_RETURN: sema_stmt_return(sema, &stmt->ret); break;
			case AST_STMT_WHILE: sema_stmt_while_loop(sema, &stmt->while_loop); break;
			case AST_STMT_IF: sema_stmt_if_else(sema, &stmt->if_else); break;
			case AST_STMT_ASSIGN: sema_stmt_assign(sema, &stmt->assign); break;
			case AST_STMT_DEFER: sema_stmt_defer(sema, &stmt->defer); break;
			case AST_STMT_INLINE_ASM: sema_stmt_inline_asm(sema, &stmt->inline_asm); break;
			case AST_STMT_EXPR: sema_value_expr_type(sema, stmt->expr, sema_expr_ctx_default()); break;
			case AST_STMT_BREAK: sema_stmt_loop_control(sema, &stmt->break_loop); break;
			case AST_STMT_CONTINUE: sema_stmt_loop_control(sema, &stmt->continue_loop); break;
		}
	}
	sema_module_pop_scope(sema);
} 
