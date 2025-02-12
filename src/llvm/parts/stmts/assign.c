#include "../../parts.h"
#include "ast/private/stmts/assign.h"

void llvm_stmt_assign(LlvmBackend *llvm, AstAssign *assign) {
	LLVMBuildStore(
		llvm->builder,
        llvm_expr(llvm, assign->expr, true),
		llvm_expr(llvm, assign->assign_expr, false)
	);
}
