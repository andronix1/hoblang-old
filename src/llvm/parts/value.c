#include "../parts.h"
#include "../utils/slices.h"

LLVMValueRef llvm_value(LlvmBackend *llvm, AstValue *value) {
	SemaScopeValueDecl *decl = value->mod_path.value;
	LLVMValueRef val = decl->llvm_value;
	for (size_t i = 0; i < vec_len(value->segments); i++) {
		AstValueSegment *seg = &value->segments[i];
		switch (seg->type) {
			case AST_VALUE_IDENT: {
				LLVMValueRef indices[] = {
					LLVMConstInt(LLVMInt32Type(), 0, false),
					LLVMConstInt(LLVMInt32Type(), seg->ident.struct_member_idx, false)
				};
				val = LLVMBuildGEP2(
					llvm->builder,
					llvm_resolve_type(i == 0 ? decl->type : value->segments[i - 1].sema_type),
					val,
					indices, 2,
					""
				);
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
				LLVMTypeRef type = llvm_resolve_type(i == 0 ? decl->type : value->segments[i - 1].sema_type);
				LLVMValueRef indices[] = {
					llvm_expr(llvm, seg->idx),
				};
				val = LLVMBuildGEP2(
					llvm->builder,
					llvm_resolve_type(seg->sema_type),
					LLVMBuildLoad2(
						llvm->builder,
						LLVMPointerType(llvm_resolve_type(seg->sema_type), 0),
						llvm_slice_ptr(llvm, type, val),
						"loaded_ptr"
					),
					indices, 1,
					""
				);
				break;
			}
		}
	}
	return val;
}
