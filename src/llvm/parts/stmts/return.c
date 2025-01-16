#include "../../parts.h"

void llvm_stmt_return(LlvmBackend *llvm, AstReturn *ret) {
	if (ret->has_value) {
		LLVMBuildRet(llvm->builder, llvm_expr(llvm, &ret->expr));
	} else {
		LLVMBuildRetVoid(llvm->builder);
	}
	llvm_pop_scope(llvm);
}
