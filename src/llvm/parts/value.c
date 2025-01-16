#include "../parts.h"

LLVMValueRef llvm_value(LlvmBackend *llvm, AstValue *value) {
	assert(value->segments[0].type == AST_VALUE_IDENT, "value must be started from ident segment");
	LLVMValueRef val = value->decl->llvm_value;
	for (size_t i = 1; i < vec_len(value->segments); i++) {
		AstValueSegment *seg = &value->segments[i];
		switch (seg->type) {
			case AST_VALUE_IDENT: {
				hob_log(LOGW, "unexpected ident in value");
				break;
			}
			case AST_VALUE_DEREF: {
				val = LLVMBuildLoad2(
					llvm->builder,
					LLVMPointerType(llvm_resolve_type(seg->sema_type), 0),
					val, 
					""
				);
				break;
			}
			case AST_VALUE_IDX: {
				LLVMValueRef indices[] = {
					llvm_expr(llvm, seg->idx),
				};
				val = LLVMBuildGEP2(
					llvm->builder,
					llvm_resolve_type(seg->sema_type),
					LLVMBuildLoad2(llvm->builder, llvm_resolve_type(value->segments[i - 1].sema_type), val, ""),
					indices, 1,
					""
				);
				break;
			}
		}
	}
	return val;
}
