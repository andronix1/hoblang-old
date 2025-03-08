#include "llvm/parts/body.h"
#include "llvm/parts/expr.h"
#include "ast/private/stmts/loop_control.h"
#include "core/vec.h"
#include "ast/private/body.h"

void llvm_stmt_assign(LlvmBackend *llvm, AstAssign *assign);
bool llvm_stmt_if(LlvmBackend *llvm, AstIfElse *if_else);
void llvm_stmt_return(LlvmBackend *llvm, AstReturn *ret);
void llvm_stmt_var(LlvmBackend *llvm, AstVar *var);
void llvm_stmt_while(LlvmBackend *llvm, AstWhile *while_loop);
void llvm_stmt_asm(LlvmBackend *llvm, AstInlineAsm *inline_asm);
void llvm_stmt_break(LlvmBackend *llvm, AstStmtLoopControl *loop_control);
void llvm_stmt_continue(LlvmBackend *llvm, AstStmtLoopControl *loop_control);

void llvm_body_break(LlvmBackend *llvm, AstDefer **defers) {
	for (size_t i = 0; i < vec_len(defers); i++) {
		llvm_body(llvm, defers[i]->body);
	}
}

bool llvm_body(LlvmBackend *llvm, AstBody *body) {
	for (size_t i = 0; i < vec_len(body->stmts); i++) {
		AstStmt *stmt = &body->stmts[i];
		switch (stmt->type) {
			case AST_STMT_VAR: llvm_stmt_var(llvm, &stmt->var); break;
			case AST_STMT_RETURN: llvm_stmt_return(llvm, &stmt->ret); return false;
			case AST_STMT_BREAK: llvm_stmt_break(llvm, &stmt->break_loop); return false;
            case AST_STMT_CONTINUE: llvm_stmt_continue(llvm, &stmt->continue_loop); return false;
			case AST_STMT_WHILE: llvm_stmt_while(llvm, &stmt->while_loop); break;
			case AST_STMT_IF: llvm_stmt_if(llvm, &stmt->if_else); break;
			case AST_STMT_ASSIGN: llvm_stmt_assign(llvm, &stmt->assign); break;
			case AST_STMT_INLINE_ASM: llvm_stmt_asm(llvm, &stmt->inline_asm); break;
			case AST_STMT_EXPR: llvm_expr(llvm, stmt->expr, false); break;
			case AST_STMT_CONST:
			case AST_STMT_DEFER:
                break;
		}
	}
	llvm_body_break(llvm, body->defers);
	return true;
}
