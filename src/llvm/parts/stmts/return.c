#include "../../parts.h"

void llvm_stmt_return(LlvmBackend *llvm, AstReturn *ret, AstBody *body) {
	llvm_body_break(llvm, body);
	if (ret->has_value) {
		LLVMBuildRet(
			llvm->builder, 
			LLVMBuildLoad2( // TODO: fix
				llvm->builder,
				llvm_resolve_type(ret->expr->sema_type),
				llvm_expr(llvm, ret->expr),
				"ret_val"
			)
		);
	} else {
		LLVMBuildRetVoid(llvm->builder);
	}
}
