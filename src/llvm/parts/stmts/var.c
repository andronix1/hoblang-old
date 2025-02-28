#include "llvm-c/Core.h"
#include "llvm-c/Types.h"
#include "llvm/private.h"
#include "llvm/parts/type.h"
#include "llvm/parts/expr.h"
#include "sema/module/decls/impl.h"
#include "ast/private/stmts/var.h"
#include "sema/module/private.h"

void llvm_stmt_var(LlvmBackend *llvm, AstVar *var) {
	LLVMTypeRef type = llvm_resolve_type(var->type.sema);
    LLVMPositionBuilderAtEnd(llvm_builder(llvm), llvm_definitions_block(llvm));
    LLVMValueRef allocated = var->decl->llvm.value = LLVMBuildAlloca(llvm_builder(llvm), type, slice_to_cstr(&var->name));
    LLVMPositionBuilderAtEnd(llvm_builder(llvm), llvm_code_block(llvm));
	if (var->initializes) {
		LLVMBuildStore(llvm_builder(llvm), llvm_expr(llvm, var->expr, true), var->decl->llvm.value);
	}
}
