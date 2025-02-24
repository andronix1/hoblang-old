#include "llvm/private.h"
#include "llvm/parts/expr.h"
#include "llvm/parts/body.h"
#include "ast/private/stmts/return.h"

void llvm_stmt_return(LlvmBackend *llvm, AstReturn *ret) {
	llvm_body_break(llvm, ret->defers);
	if (ret->has_value) {
		LLVMBuildRet(
			llvm_builder(llvm), 
			llvm_expr(llvm, ret->expr, true)
		);
	} else {
		LLVMBuildRetVoid(llvm_builder(llvm));
	}
}
