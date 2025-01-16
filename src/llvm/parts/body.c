#include "../parts.h"

void llvm_stmt_assign(LlvmBackend *llvm, AstAssign *assign);
void llvm_stmt_if(LlvmBackend *llvm, AstIfElse *if_else);
void llvm_stmt_return(LlvmBackend *llvm, AstReturn *ret);
void llvm_stmt_var(LlvmBackend *llvm, AstVar *var);
void llvm_stmt_while(LlvmBackend *llvm, AstWhile *while_loop);

bool llvm_body(LlvmBackend *llvm, AstBody *body) {
	for (size_t i = 0; i < vec_len(body->stmts); i++) {
		AstStmt *stmt = &body->stmts[i];
		switch (stmt->type) {
			case AST_STMT_VAR: llvm_stmt_var(llvm, &stmt->var); break;
			case AST_STMT_RETURN: llvm_stmt_return(llvm, &stmt->ret); return false;
			case AST_STMT_FUNC_CALL: llvm_func_call(llvm, &stmt->func_call); break;
			case AST_STMT_WHILE: llvm_stmt_while(llvm, &stmt->while_loop); break;
			case AST_STMT_IF: llvm_stmt_if(llvm, &stmt->if_else); break;
			case AST_STMT_ASSIGN: llvm_stmt_assign(llvm, &stmt->assign); break;
		}
	}
	return true;
}
