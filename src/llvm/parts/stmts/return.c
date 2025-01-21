#include "../../parts.h"

void llvm_stmt_return(LlvmBackend *llvm, AstReturn *ret, AstBody *body) {
	llvm_body_break(llvm, body);
	if (ret->has_value) {
		LLVMBuildRet(llvm->builder, llvm_expr(llvm, &ret->expr));
	} else {
		LLVMBuildRetVoid(llvm->builder);
	}
}
