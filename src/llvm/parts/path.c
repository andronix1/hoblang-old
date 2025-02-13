#include "../parts.h"
#include "core/vec.h"
#include "../utils/slices.h"
#include "ast/private/path.h"
#include "sema/module/private.h"
#include "sema/module/decls/impl.h"

LLVMValueRef llvm_resolve_inner_path(LlvmBackend *llvm, LLVMValueRef value, AstInnerPath *path) {
    for (size_t i = 0; i < vec_len(path->segments); i++) {
        SemaInnerPath *segment = &path->segments[i].sema;
        switch (segment->type) {
            case SEMA_INNER_PATH_DEREF:
                value = LLVMBuildLoad2(
                    llvm->builder,
                    LLVMPointerType(llvm_resolve_type(segment->deref_type), 0),
                    value,
                    "deref"
                );
                break;
            case SEMA_INNER_PATH_STRUCT_MEMBER:
                LLVMValueRef indices[] = {
                    LLVMConstInt(LLVMInt32Type(), 0, false),
                    LLVMConstInt(LLVMInt32Type(), segment->struct_member.idx, false)
                };
                value = LLVMBuildGEP2(
                    llvm->builder,
                    llvm_resolve_type(segment->struct_member.of),
                    value,
                    indices, 2,
                    "struct_member"
                );
                break;
            case SEMA_INNER_PATH_SLICE_RAW:
                value = llvm_slice_ptr(llvm, llvm_resolve_type(segment->slice_type), value);
                break;
            case SEMA_INNER_PATH_SLICE_LEN:
                value = llvm_slice_len(llvm, llvm_resolve_type(segment->slice_type), value);
                break;
        }
    }
    return value;
}

LLVMValueRef llvm_resolve_decl_path(LlvmBackend *llvm, AstDeclPath *path) {
	LLVMValueRef result = path->decl->value_decl.llvm_value;
	assert(result, "llvm_value is not mapped in {ast::dpath}", path);
	return result;
}

LLVMValueRef llvm_resolve_path(LlvmBackend *llvm, AstPath *path) {
    return llvm_resolve_inner_path(
        llvm,
        llvm_resolve_decl_path(llvm, &path->decl_path),
        &path->inner_path
    );
}
