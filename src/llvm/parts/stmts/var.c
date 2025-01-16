#include "../../parts.h"

void llvm_stmt_var(LlvmBackend *llvm, AstVar *var) {
	LLVMTypeRef type = llvm_resolve_type(var->type.sema);
	LLVMValueRef alloced = LLVMBuildAlloca(llvm->builder, type, slice_to_cstr(&var->name));
	if (var->initializes) {
		LLVMBuildStore(llvm->builder, llvm_expr(llvm, &var->expr), alloced);
	}
	LlvmValue value = {
		.type = type,
		.value = alloced,
		.name = &var->name,
	};
	llvm_push_value(llvm, &value);
}
