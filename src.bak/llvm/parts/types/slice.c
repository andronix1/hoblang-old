#include "llvm/private.h"
#include "llvm/parts/types/slice.h"
#include "llvm/utils/alloca.h"
#include "llvm/utils/member.h"
#include <llvm-c/Core.h>

LLVMTypeRef llvm_slice_type(LLVMTypeRef of) {
    LLVMTypeRef fields[2] = { LLVMInt64Type(), LLVMPointerType(of, 0) };
    return LLVMStructType(fields, 2, false);
}

LLVMValueRef llvm_slice_len(LlvmBackend *llvm, LLVMTypeRef type, LLVMValueRef slice, bool load) {
    return llvm_get_member(llvm, type, LLVMInt32Type(), slice, 0, load);
}

LLVMValueRef llvm_slice_ptr(LlvmBackend *llvm, LLVMTypeRef of, LLVMValueRef slice, bool load) {
    return llvm_get_member(llvm, llvm_slice_type(of), LLVMPointerType(of, 0), slice, 1, load);
}

LLVMValueRef llvm_alloca_slice(LlvmBackend *llvm, LLVMTypeRef of, LLVMValueRef ptr, size_t len) {
    LLVMTypeRef slice_type = llvm_slice_type(of);
    LLVMValueRef slice = llvm_alloca(llvm, slice_type);
    LLVMBuildStore(llvm_builder(llvm), LLVMConstInt(LLVMInt64Type(), len, false), llvm_slice_len(llvm, slice_type, slice, false));
    LLVMBuildStore(llvm_builder(llvm), ptr, llvm_slice_ptr(llvm, slice_type, slice, false));
    return LLVMBuildLoad2(llvm_builder(llvm), slice_type, slice, "loaded_slice");
}

LLVMValueRef llvm_slice_from_array_ptr(LlvmBackend *llvm, LLVMTypeRef of, LLVMValueRef array, size_t len) {
	LLVMValueRef indices[] = {
		LLVMConstInt(LLVMInt32Type(), 0, false),
		LLVMConstInt(LLVMInt32Type(), 0, false)
	};
	LLVMValueRef array_ptr = LLVMBuildGEP2(
		llvm_builder(llvm),
		of,
		array,
		indices, 2,
		"array_ptr"
	);

    return llvm_alloca_slice(llvm, of, array_ptr, len);
}

LLVMValueRef llvm_slice_from_str(LlvmBackend *llvm, Slice *str) {
    LLVMValueRef str_global = LLVMAddGlobal(llvm_current_module(llvm), LLVMArrayType(LLVMInt8Type(), str->len), "str");
    LLVMSetInitializer(str_global, LLVMConstString(str->str, str->len, true));
    LLVMValueRef str_ptr = LLVMBuildBitCast(llvm_builder(llvm), str_global, LLVMPointerType(LLVMInt8Type(), 0), "");

    return llvm_alloca_slice(llvm, LLVMInt8Type(), str_ptr, str->len);
}
