#include "../../parts.h"
#include "sema/module/private.h"

void llvm_stmt_var(LlvmBackend *llvm, AstVar *var) {
	LLVMTypeRef type = llvm_resolve_type(var->type.sema);
	var->decl->llvm_value = LLVMBuildAlloca(llvm->builder, type, slice_to_cstr(&var->name));
	if (var->initializes) {
		LLVMBuildStore(llvm->builder, llvm_expr(llvm, var->expr, true), var->decl->llvm_value);
	}
}
