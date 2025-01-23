#include "../parts.h"

LLVMValueRef llvm_sema_value(LlvmBackend *llvm, SemaValueDecl *decl) {
	assert(decl, "value was not mapped to decl");
	if (decl->llvm_value) {
		return decl->llvm_value;
	}
	assert(decl->type, "type was not mapped to decl's value");
	switch (decl->type->type) {
		case SEMA_TYPE_FUNCTION: {
			LLVMTypeRef *params = alloca(sizeof(LLVMTypeRef) * vec_len(decl->type->func.args));
			for (size_t i = 0; i < vec_len(decl->type->func.args); i++) {
				params[i] = llvm_resolve_type(decl->type->func.args[i].type.sema);
			}
			LLVMTypeRef type = LLVMFunctionType(llvm_resolve_type(decl->type->func.returning), params, vec_len(decl->type->func.args), false);
			return decl->llvm_value = LLVMAddFunction(llvm->module, "", type);
		}

		case SEMA_TYPE_STRUCT:
		case SEMA_TYPE_PRIMITIVE:
		case SEMA_TYPE_POINTER:
			break;
	}
	assert(0, "xref was not caught!");
}

LLVMValueRef llvm_value(LlvmBackend *llvm, AstValue *value) {
	SemaValueDecl *decl = value->mod_path.value;
	LLVMValueRef val = decl->llvm_value ? decl->llvm_value : llvm_sema_value(llvm, decl);
	for (size_t i = 0; i < vec_len(value->segments); i++) {
		AstValueSegment *seg = &value->segments[i];
		switch (seg->type) {
			case AST_VALUE_IDENT: {
				LLVMValueRef indices[] = {
					LLVMConstInt(LLVMInt32Type(), seg->ident.struct_member_idx, false)
				};
				val = LLVMBuildGEP2(
					llvm->builder,
					llvm_resolve_type(seg->ident.struct_sema_type),
					val,
					indices, 1,
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
				LLVMValueRef indices[] = {
					llvm_expr(llvm, seg->idx),
				};
				val = LLVMBuildGEP2(
					llvm->builder,
					llvm_resolve_type(seg->sema_type),
					LLVMBuildLoad2(llvm->builder, llvm_resolve_type(i == 0 ? decl->type :  value->segments[i - 1].sema_type), val, ""),
					indices, 1,
					""
				);
				break;
			}
		}
	}
	return val;
}
