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
				// LLVMValueRef indices_ptr[] = {
				// 	LLVMConstInt(LLVMInt32Type(), 0, false),
				// 	LLVMConstInt(LLVMInt32Type(), 1, false),
				// 	//llvm_expr(llvm, seg->idx),
				// };
				LLVMTypeRef type = llvm_resolve_type(i == 0 ? decl->type : value->segments[i - 1].sema_type);
				// LLVMValueRef slice_ptr = LLVMBuildGEP2(
				// 	llvm->builder,
				// 	type,
				// 	val, // LLVMBuildLoad2(llvm->builder, type, val, "idx_load"),
				// 	indices_ptr, 2,
				// 	"idx_val"
				// );
				// LLVMValueRef raw_ptr = LLVMBuildLoad2(llvm->builder, LLVMPointerType(llvm_resolve_type(seg->sema_type), 0), slice_ptr, "");
				LLVMValueRef indices[] = {
					LLVMConstInt(LLVMInt32Type(), 0, false),
					// LLVMConstInt(LLVMInt32Type(), 1, false),
					// LLVMConstInt(LLVMInt32Type(), 0, false),
					llvm_expr(llvm, seg->idx),
				};
				val = LLVMBuildGEP2(
					llvm->builder,
					llvm_resolve_type(seg->sema_type),
					LLVMBuildExtractValue(
						llvm->builder,
						LLVMBuildLoad2(
							llvm->builder,
							type,
							// llvm_slice_ptr(llvm, type, val),
							val,
							""
						),
						1,
						"ptr"
					),
					indices, 2,
					"idx_val"
				);
				break;
			}
		}
	}
	return val;
}
